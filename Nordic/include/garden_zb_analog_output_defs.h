#ifndef _GARDEN_ZB_ANALOG_OUTPUT_DEFS_
#define _GARDEN_ZB_ANALOG_OUTPUT_DEFS_

#include <zboss_api.h>
#include <zcl/zb_zcl_common.h>

// Most defines in this file are updated from the ZB_ZCL_DECLARE_TEMP_MEASUREMENT_ATTRIB_LIST,
// adapting attributes and IDs to match the mSoilMoisture cluster spec.1
// Values from https://github.com/Koenkk/zigbee-herdsman/blob/master/src/zcl/definition/cluster.ts#L2570
// (msSoilMoisture).
// Cluster attributes definitions in https://www.st.com/resource/en/user_manual/um2977-stm32wb-series-zigbee-cluster-library-api-stmicroelectronics.pdf.

//#define GARDEN_ZB_ZCL_ATTR_ANALOG_OUTPUT_CLUSTER_ID 1032
#define GARDEN_ZB_ZCL_ATTR_ANALOG_OUTPUT_CLUSTER_ID ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT

// Soil moisture value represented as an uint16.
#define GARDEN_ZB_ZCL_ATTR_ANALOG_OUTPUT_VALUE_ID 0x0055

// Required callbacks. ZBOSS will call these.
void garden_zcl_analog_output_init_server(void);
void garden_zcl_analog_output_init_client(void);

#define GARDEN_ZB_ZCL_ATTR_ANALOG_OUTPUT_CLUSTER_ID_SERVER_ROLE_INIT garden_zcl_analog_output_init_server
#define GARDEN_ZB_ZCL_ATTR_ANALOG_OUTPUT_CLUSTER_ID_CLIENT_ROLE_INIT garden_zcl_analog_output_init_client


#define GARDEN_ZB_ZCL_ANALOG_OUTPUT_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0042)

// for the first sensor
#define ZB_SET_ATTR_DESCR_WITH_GARDEN_ZB_ZCL_ATTR_ANALOG_OUTPUT_VALUE_ID(data_ptr) \
  {                                                                              \
    GARDEN_ZB_ZCL_ATTR_ANALOG_OUTPUT_VALUE_ID,                                   \
        ZB_ZCL_ATTR_TYPE_U16,                                                    \
        ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,             \
        (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                      \
        (void*)data_ptr                                                          \
  }


#define GARDEN_ZB_ZCL_DECLARE_ANALOG_OUTPUT_ATTRIB_LIST(attr_list, value)                   \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, GARDEN_ZB_ZCL_ANALOG_OUTPUT) \
  ZB_ZCL_SET_ATTR_DESC(GARDEN_ZB_ZCL_ATTR_ANALOG_OUTPUT_VALUE_ID, (value))                  \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

//*
void garden_zcl_analog_output_init_server(void) {
  zb_zcl_add_cluster_handlers(GARDEN_ZB_ZCL_ATTR_ANALOG_OUTPUT_CLUSTER_ID,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              /*cluster_check_value=*/NULL,
                              /*cluster_write_attr_hook=*/NULL,
                              /*cluster_handler=*/NULL);
}
//*/
void garden_zcl_analog_output_init_client(void) {
  // Nothing.
}

#endif  // _GARDEN_ZB_ANALOG_OUTPUT_DEFS_


