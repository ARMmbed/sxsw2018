let TTN_APP_ID = 'YOUR_APP_ID';
let TTN_ACCESS_KEY = 'YOUR_ACCESS_KEY';

const express = require('express');
const app = express();
const server = require('http').Server(app);
const io = require('socket.io')(server);
const hbs = require('hbs');
const ttn = require('ttn');
const fs = require('fs');
const Path = require('path');

// improved database
const dbFile = Path.join(__dirname, 'db.json');

// Some options for express (node.js web app library)
hbs.registerPartials(__dirname + '/views/partials');
app.use(express.static(__dirname + '/public'));
app.set('view engine', 'html');
app.set('views', __dirname + '/views');
app.engine('html', hbs.__express);

// Store some state about all applications
let applications = {};

// Store some state about all devices
let devices = {};

if (fs.existsSync(dbFile)) {
    console.time('LoadingDB');
    let db = JSON.parse(fs.readFileSync(dbFile, 'utf-8'));
    devices = db.devices;
    for (appId in db.applications) {
        if (db.applications.hasOwnProperty(appId)) {
            connectApplication(appId, db.applications[appId]);
        }
    }
    console.timeEnd('LoadingDB');
}

// And handle requests
app.get('/', function (req, res, next) {
    let d = Object.keys(devices).map(k => {
        let keys = k.split(/\:/g);
        let o = {
            appId: keys[0],
            devId: keys[1],
            eui: devices[k].eui,
            lat: devices[k].lat,
            lng: devices[k].lng,
            particles: devices[k].particles.slice(Math.max(devices[k].particles.length - 30, 1)), // take last 30
        };
        return o;
    })
    // Render index view, with the devices based on mapToView function
    res.render('index', { devices: JSON.stringify(d) });
});

io.on('connection', socket => {
    socket.on('connect-application', (appId, accessKey) => {
        console.log('Connecting to application', appId, accessKey)
        connectApplication(appId, accessKey);
    });

    socket.on('location-change', (appId, devId, lat, lng) => {
        let key = appId + ':' + devId;
        if (!devices[key]) {
            console.error('Device not found', appId, devId);
            return;
        }

        console.log('Location changed', appId, devId, lat, lng);

        let d = devices[key];
        d.lat = lat;
        d.lng = lng;

        io.emit('location-change', {
            appId: appId,
            devId: devId,
            eui: d.eui,
            lat: d.lat,
            lng: d.lng
        }, lat, lng);
    });
});

server.listen(process.env.PORT || 5270, process.env.HOST || '0.0.0.0', function () {
    console.log('Web server listening on port %s!', process.env.PORT || 5270);
});

function connectApplication(appId, accessKey) {
    if (applications[appId]) {
        if (!applications[appId].client) {
            console.log('Already connecting to app %s', appId)
            return;
        }
        applications[appId].client.close();
        delete applications[appId];
    }

    applications[appId] = {
        accessKey: accessKey
    }

    console.log('Connecting to the The Things Network data channel for app %s...', appId);
    ttn.data(appId, accessKey).then(client => {
        applications[appId].client = client;
        
        client.on('uplink', (devId, payload) => {
            // on device side we did /100, so *100 here to normalize
            if (payload.payload_fields.analog_in_1) {
                payload.payload_fields.analog_in_1 *= 100;
            }

            console.log('Received uplink', appId, devId, payload.payload_fields.analog_in_1);
    
            let key = appId + ':' + devId;
            let d = devices[key] = devices[key] || {};
            d.eui = payload.hardware_serial;
    
            if (!d.lat) {
                d.lat = 30.2672 + (Math.random() / 10 - 0.05);
            }
            if (!d.lng) {
                d.lng = -97.7341 + (Math.random() / 10 - 0.05);
            }
    
            if (payload.payload_fields.analog_in_1) {
                d.particles = d.particles || [];
                d.particles.push({
                    ts: new Date(payload.metadata.time),
                    value: payload.payload_fields.analog_in_1
                });
    
                io.emit('particle-change', {
                    appId: appId,
                    devId: devId,
                    eui: d.eui,
                    lat: d.lat,
                    lng: d.lng
                }, payload.metadata.time, payload.payload_fields.analog_in_1);
            }
        });
    
        console.log('Connected to The Things Network data channel for app %s', appId);
    }).catch(err => {
        console.error('Could not connect to The Things Network app %s...', appId, err);
    });
}

connectApplication(process.env.TTN_APP_ID || TTN_APP_ID, process.env.TTN_ACCESS_KEY || TTN_ACCESS_KEY);

function exitHandler(options) {
    let db = {
        devices: devices,
        applications: {}
    }
    for (appId in applications) {
        if (applications.hasOwnProperty(appId)) {
            db.applications[appId] = applications[appId].accessKey;
        }
    }
    fs.writeFileSync(dbFile, JSON.stringify(db), 'utf-8');

    if (options.exit) {
        process.exit();
    }
}

process.on('exit', exitHandler.bind(null, { cleanup: true }));
process.on('SIGINT', exitHandler.bind(null, { exit: true }));
process.on('SIGUSR1', exitHandler.bind(null, { exit: true }));
process.on('SIGUSR2', exitHandler.bind(null, { exit: true }));
process.on('uncaughtException', exitHandler.bind(null, { exit: true }));
