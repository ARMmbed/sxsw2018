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

TTN_APP_ID = process.env['TTN_APP_ID'] || TTN_APP_ID;
TTN_ACCESS_KEY = process.env['TTN_ACCESS_KEY'] || TTN_APP_ID;

// Store some state about all devices, you probably want to store this in a database
let devices = {};

if (fs.existsSync(dbFile)) {
    devices = JSON.parse(fs.readFileSync(dbFile, 'utf-8'));
}

// And handle requests
app.get('/', function (req, res, next) {
    let d = Object.keys(devices).map(k => {
        let o = {
            devId: k,
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
    socket.on('location-change', (devId, lat, lng) => {
        if (!devices[devId]) return;

        console.log('loc-change', devId, lat, lng);

        devices[devId].lat = lat;
        devices[devId].lng = lng;

        let d = devices[devId];

        io.emit('location-change', {
            devId: devId,
            eui: d.eui,
            lat: d.lat,
            lng: d.lng
        }, lat, lng);
    });
});



// Connect to TTN
console.log('Connecting to the The Things Network data channel...');

ttn.data(TTN_APP_ID, TTN_ACCESS_KEY).then(client => {
    client.on('uplink', (devId, payload) => {
        console.log('retrieved uplink', devId, payload.payload_fields.analog_in_1);

        let d = devices[devId] = devices[devId] || {};
        d.eui = payload.hardware_serial;

        if (!d.lat) {
            d.lat = 30.2672 + (Math.random() / 10 - 0.05);
            d.lng = -97.7341 + (Math.random() / 10 - 0.05);
        }

        if (payload.payload_fields.analog_in_1) {
            d.particles = d.particles || [];
            d.particles.push({
                ts: new Date(payload.metadata.time),
                value: payload.payload_fields.analog_in_1
            });

            io.emit('particle-change', {
                devId: devId,
                eui: d.eui,
                lat: d.lat,
                lng: d.lng
            }, payload.metadata.time, payload.payload_fields.analog_in_1);
        }
    });

    console.log('Connected to The Things Network data channel');
}).then(() => {
    // Now we can start the web server
    server.listen(process.env.PORT || 5270, process.env.HOST || '0.0.0.0', function () {
        console.log('Web server listening on port %s!', process.env.PORT || 5270);
    });
}).catch(err => {
    console.error('Could not authenticate with TTN...', err);
});

function exitHandler(options) {
    fs.writeFileSync(dbFile, JSON.stringify(devices), 'utf-8');

    if (options.exit) process.exit();
}

process.on('exit', exitHandler.bind(null, { cleanup:true }));
process.on('SIGINT', exitHandler.bind(null, { exit:true }));
process.on('SIGUSR1', exitHandler.bind(null, { exit:true }));
process.on('SIGUSR2', exitHandler.bind(null, { exit:true }));
process.on('uncaughtException', exitHandler.bind(null, { exit:true }));

