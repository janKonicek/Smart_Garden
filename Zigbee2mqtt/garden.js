const fz = require("zigbee-herdsman-converters/converters/fromZigbee");
const tz = require("zigbee-herdsman-converters/converters/toZigbee");
const exposes = require("zigbee-herdsman-converters/lib/exposes");
const reporting = require("zigbee-herdsman-converters/lib/reporting");
const extend = require("zigbee-herdsman-converters/lib/extend");
const constants = require("zigbee-herdsman-converters/lib/constants");
const e = exposes.presets;
const ea = exposes.access;

const definition = {
    zigbeeModel: ["Garden_v0.2b"],
    model: "Garden_v0.2b",
    vendor: "Honza",
    description: "description",
    fromZigbee: [fz.on_off, fz.curtain_position_analog_output],
    toZigbee: [tz.on_off], // Should be empty, unless device can be controlled (e.g. lights, switches).
    exposes: [e.identify(), e.switch(), e.numeric()],
    configure: async (device, coordinatorEndpoint, logger) => {
        const endpointID = 11;
        const endpoint = device.getEndpoint(endpointID);
        const clusters = ["genOnOff", "genAnalogOutput"];
        await reporting.bind(endpoint, coordinatorEndpoint, clusters);
        //await reporting.temperature(endpoint, {min: 1, max: constants.repInterval.MINUTES_5, change: 10}); // 0.1 degree change
        //await reporting.humidity(endpoint, {min: 1, max: constants.repInterval.MINUTES_5, change: 10}); // 0.1 % change
        //await reporting.co2(endpoint, {min: 5, max: constants.repInterval.MINUTES_5, change: 0.00005}); // 50 ppm change
    },
};

module.exports = definition;