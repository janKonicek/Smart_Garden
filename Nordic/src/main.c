/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *
 * @brief Zigbee application template.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>

#include "garden_zb_attrs.h"
#include "garden_zb_endpoint_defs.h"
#include "garden_zb_soil_moisture_defs.h"
#include "garden_zb_analog_output_defs.h"

#include <addons/zcl/zb_zcl_temp_measurement_addons.h>

#include <zboss_api.h>
#include <zigbee/zigbee_error_handler.h>
#include <zigbee/zigbee_app_utils.h>
#include <zb_nrf_platform.h>

/* Device endpoint, used to receive ZCL commands. */
#define APP_TEMPLATE_ENDPOINT               11


/* Version of the application software (1 byte). */
#define TEMPLATE_INIT_BASIC_APP_VERSION     01

/* Version of the implementation of the Zigbee stack (1 byte). */
#define TEMPLATE_INIT_BASIC_STACK_VERSION   10

/* Version of the hardware of the device (1 byte). */
#define TEMPLATE_INIT_BASIC_HW_VERSION      11

/* Manufacturer name (32 bytes). */
#define TEMPLATE_INIT_BASIC_MANUF_NAME      "Nordic"

/* Model number assigned by manufacturer (32-bytes long string). */
#define TEMPLATE_INIT_BASIC_MODEL_ID        "Garden_v0.2b"

/* First 8 bytes specify the date of manufacturer of the device
 * in ISO 8601 format (YYYYMMDD). The rest (8 bytes) are manufacturer specific.
 */
#define TEMPLATE_INIT_BASIC_DATE_CODE       "20200329"

/* Type of power sources available for the device.
 * For possible values see section 3.2.2.2.8 of ZCL specification.
 */
#define TEMPLATE_INIT_BASIC_POWER_SOURCE    ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE

/* Describes the physical location of the device (16 bytes).
 * May be modified during commissioning process.
 */
#define TEMPLATE_INIT_BASIC_LOCATION_DESC   "Office desk"

/* Describes the type of physical environment.
 * For possible values see section 3.2.2.2.10 of ZCL specification.
 */
#define TEMPLATE_INIT_BASIC_PH_ENV          ZB_ZCL_BASIC_ENV_UNSPECIFIED


/* Type of power sources available for the device.
 * For possible values see section 3.2.2.2.8 of ZCL specification.
 */
#define TEMPLATE_INIT_BASIC_POWER_SOURCE    ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE

/* LED indicating that device successfully joined Zigbee network. */
#define ZIGBEE_NETWORK_STATE_LED            DK_LED3

/* LED used for device identification. */
#define IDENTIFY_LED                        DK_LED4

/* Button used to enter the Identify mode. */
#define IDENTIFY_MODE_BUTTON                DK_BTN4_MSK

/* Button to start Factory Reset */
#define FACTORY_RESET_BUTTON                IDENTIFY_MODE_BUTTON

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

/* Main application customizable context.
 * Stores all settings and static values.
 */
/*
struct zb_device_ctx {
	zb_zcl_basic_attrs_t basic_attr;
	zb_zcl_identify_attrs_t identify_attr;
};
//*/

/* Zigbee device application context storage. */
static struct zb_device_ctx dev_ctx;

ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(
	identify_attr_list,
	&dev_ctx.identify_attr.identify_time);

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(
	basic_attr_list,
	&dev_ctx.basic_attr.zcl_version,
	&dev_ctx.basic_attr.app_version,
	&dev_ctx.basic_attr.stack_version,
	&dev_ctx.basic_attr.hw_version,
	dev_ctx.basic_attr.mf_name,
	dev_ctx.basic_attr.model_id,
	dev_ctx.basic_attr.date_code,
	&dev_ctx.basic_attr.power_source,
	dev_ctx.basic_attr.location_id,
	&dev_ctx.basic_attr.ph_env,
	dev_ctx.basic_attr.sw_ver);


//*
GARDEN_ZB_ZCL_DECLARE_SOIL_MOISTURE_ATTRIB_LIST(
    soil_moisture_attr_list,
    &dev_ctx.soil_moisture_attrs.percentage1,
	&dev_ctx.soil_moisture_attrs.percentage2);
//*/

//*
ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST(
	on_off_attr_list,
	&dev_ctx.on_off_attr.on_off);
//*/

GARDEN_ZB_ZCL_DECLARE_ANALOG_OUTPUT_ATTRIB_LIST(
	analog_output_attr_list,
	&dev_ctx.analog_output_attr.value);


GARDEN_ZB_DECLARE_CLUSTER_LIST(
    app_template_clusters,
    basic_attr_list,
    identify_attr_list,
    on_off_attr_list,
	analog_output_attr_list,
    soil_moisture_attr_list);

GARDEN_ZB_DECLARE_ENDPOINT(
    app_template_ep,
    GARDEN_ZIGBEE_ENDPOINT,
    app_template_clusters);

/*
ZB_DECLARE_RANGE_EXTENDER_CLUSTER_LIST(
	app_template_clusters,
	basic_attr_list,
	identify_attr_list);


ZB_DECLARE_RANGE_EXTENDER_EP(
	app_template_ep,
	APP_TEMPLATE_ENDPOINT,
	app_template_clusters);
//*/


ZBOSS_DECLARE_DEVICE_CTX_1_EP(
	app_template_ctx,
	app_template_ep);









/**@brief Function for initializing all clusters attributes. */
static void app_clusters_attr_init(void)
{
	dev_ctx.basic_attr.zcl_version = ZB_ZCL_VERSION;
	dev_ctx.basic_attr.power_source = ZB_ZCL_BASIC_POWER_SOURCE_BATTERY;
	ZB_ZCL_SET_STRING_VAL(
		dev_ctx.basic_attr.mf_name,
		GARDEN_BASIC_MANUF_NAME,
		ZB_ZCL_STRING_CONST_SIZE(GARDEN_BASIC_MANUF_NAME));

	ZB_ZCL_SET_STRING_VAL(
		dev_ctx.basic_attr.model_id,
		TEMPLATE_INIT_BASIC_MODEL_ID,
		ZB_ZCL_STRING_CONST_SIZE(TEMPLATE_INIT_BASIC_MODEL_ID));

	ZB_ZCL_SET_STRING_VAL(
		dev_ctx.basic_attr.date_code,
		TEMPLATE_INIT_BASIC_DATE_CODE,
		ZB_ZCL_STRING_CONST_SIZE(TEMPLATE_INIT_BASIC_DATE_CODE));

	dev_ctx.basic_attr.hw_version = TEMPLATE_INIT_BASIC_HW_VERSION;

	//dev_ctx->batt_attrs.quantity = 1;
	//dev_ctx->batt_attrs.size = ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_OTHER;

	dev_ctx.identify_attr.identify_time =
		ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;
}

/**@brief Function to toggle the identify LED
 *
 * @param  bufid  Unused parameter, required by ZBOSS scheduler API.
 */
static void toggle_identify_led(zb_bufid_t bufid)
{
	static int blink_status;

	dk_set_led(IDENTIFY_LED, (++blink_status) % 2);
	ZB_SCHEDULE_APP_ALARM(toggle_identify_led, bufid, ZB_MILLISECONDS_TO_BEACON_INTERVAL(100));
}

/**@brief Function to handle identify notification events on the first endpoint.
 *
 * @param  bufid  Unused parameter, required by ZBOSS scheduler API.
 */
static void identify_cb(zb_bufid_t bufid)
{
	zb_ret_t zb_err_code;

	if (bufid) {
		/* Schedule a self-scheduling function that will toggle the LED */
		ZB_SCHEDULE_APP_CALLBACK(toggle_identify_led, bufid);
	} else {
		/* Cancel the toggling function alarm and turn off LED */
		zb_err_code = ZB_SCHEDULE_APP_ALARM_CANCEL(toggle_identify_led, ZB_ALARM_ANY_PARAM);
		ZVUNUSED(zb_err_code);

		dk_set_led(IDENTIFY_LED, 0);
	}
}

/**@brief Starts identifying the device.
 *
 * @param  bufid  Unused parameter, required by ZBOSS scheduler API.
 */
static void start_identifying(zb_bufid_t bufid)
{
	ZVUNUSED(bufid);

	if (ZB_JOINED()) {
		/* Check if endpoint is in identifying mode,
		 * if not put desired endpoint in identifying mode.
		 */
		if (dev_ctx.identify_attr.identify_time ==
		ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE) {

			zb_ret_t zb_err_code = zb_bdb_finding_binding_target(
				APP_TEMPLATE_ENDPOINT);

			if (zb_err_code == RET_OK) {
				LOG_INF("Enter identify mode");
			} else if (zb_err_code == RET_INVALID_STATE) {
				LOG_WRN("RET_INVALID_STATE - Cannot enter identify mode");
			} else {
				ZB_ERROR_CHECK(zb_err_code);
			}
		} else {
			LOG_INF("Cancel identify mode");
			zb_bdb_finding_binding_target_cancel();
		}
	} else {
		LOG_WRN("Device not in a network - cannot enter identify mode");
	}
}

/**@brief Callback for button events.
 *
 * @param[in]   button_state  Bitmask containing buttons state.
 * @param[in]   has_changed   Bitmask containing buttons
 *                            that have changed their state.
 */
static void button_changed(uint32_t button_state, uint32_t has_changed)
{
	if (IDENTIFY_MODE_BUTTON & has_changed) {
		if (IDENTIFY_MODE_BUTTON & button_state) {
			/* Button changed its state to pressed */
		} else {
			/* Button changed its state to released */
			if (was_factory_reset_done()) {
				/* The long press was for Factory Reset */
				LOG_DBG("After Factory Reset - ignore button release");
			} else   {
				/* Button released before Factory Reset */

				/* Start identification mode */
				ZB_SCHEDULE_APP_CALLBACK(start_identifying, 0);
			}
		}
	}

	check_factory_reset_button(button_state, has_changed);
}

/**@brief Function for initializing LEDs and Buttons. */
static void configure_gpio(void)
{
	int err;

	err = dk_buttons_init(button_changed);
	if (err) {
		LOG_ERR("Cannot init buttons (err: %d)", err);
	}

	err = dk_leds_init();
	if (err) {
		LOG_ERR("Cannot init LEDs (err: %d)", err);
	}
}

/**@brief Zigbee stack event handler.
 *
 * @param[in]   bufid   Reference to the Zigbee stack buffer
 *                      used to pass signal.
 */
void zboss_signal_handler(zb_bufid_t bufid)
{
	/* Update network status LED. */
	zigbee_led_status_update(bufid, ZIGBEE_NETWORK_STATE_LED);

	/* No application-specific behavior is required.
	 * Call default signal handler.
	 */
	ZB_ERROR_CHECK(zigbee_default_signal_handler(bufid));

	/* All callbacks should either reuse or free passed buffers.
	 * If bufid == 0, the buffer is invalid (not passed).
	 */
	if (bufid) {
		zb_buf_free(bufid);
	}
}

int main(void)
{
	LOG_INF("Starting Zigbee application template example");

	/* Initialize */
	configure_gpio();
	register_factory_reset_button(FACTORY_RESET_BUTTON);

	/* Register device context (endpoints). */
	ZB_AF_REGISTER_DEVICE_CTX(&app_template_ctx);

	app_clusters_attr_init();

	/* Register handlers to identify notifications */
	ZB_AF_SET_IDENTIFY_NOTIFICATION_HANDLER(APP_TEMPLATE_ENDPOINT, identify_cb);

	/* Start Zigbee default thread */
	zigbee_enable();

	LOG_INF("Zigbee application template started");

	return 0;
}
