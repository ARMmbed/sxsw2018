# Changing the World with Open, Long-Range IoT

>TODO: Arm Mbed Blinky

>TODO: Import LoRaWAN example. Stop where you need to enter LoRaWAN keys

## Connecting to The Things Network

### Setting up

1. Go to [The Things Network Console](https://console.thethingsnetwork.org)
2. Login with your account or click [Create an account](https://account.thethingsnetwork.org/register)
   
   ![console](media/console.png)

   >The Console allows you to manage Applications and Gateways.

3. Click **Applications**
4. Click **Add application**
5. Enter a **Application ID** and **Description**, this can be anything
6. Be sure to select `ttn-handler-us-west` in **Handler registration**

   ![add-application](media/add-application.png)

   >The Things Network is a global and distributed network. Selecting the Handler that is closest to you and your gateways allows for higher response times.

7. Click **Add application**

   ![application-overview](media/application-overview.png)

### Registering your Device

1. In your application, go to **Devices**
2. Click **register device**
3. Enter a **Device ID**
4. Look very closely at the Multi-Tech xDot on your L-Tek FF1705, the **Device EUI** is printed after **NODE**:

   ![node-eui](media/node-eui.jpg)

   >The EUI starts with `00:80:00:00:...`. Enter without the colons.

   ![register-device](media/register-device.png)

   >You can leave the Application EUI to be generated automatically.

5. Click **Register**

   ![device-overview](media/device-overview.png)

   >Your device needs to be programmed with the **Application EUI** and **App Key**

7. Click the `< >` button of the **Application EUI** and **App Key** values to show the value as C-style array
8. Click the **Copy** button on the right of the value to copy to clipboard

   ![copy-appeui](media/copy-appeui.png)

>TODO: Paste this in Arm Mbed OS