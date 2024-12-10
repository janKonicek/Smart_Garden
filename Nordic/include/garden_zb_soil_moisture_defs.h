#ifndef _GARDEN_ZB_SOIL_MOISTURE_DEFS_
#define _GARDEN_ZB_SOIL_MOISTURE_DEFS_

#include <zboss_api.h>
#include <zcl/zb_zcl_common.h>

// Most defines in this file are updated from the ZB_ZCL_DECLARE_TEMP_MEASUREMENT_ATTRIB_LIST,
// adapting attributes and IDs to match the mSoilMoisture cluster spec.
// Values from https://github.com/Koenkk/zigbee-herdsman/blob/master/src/zcl/definition/cluster.ts#L2570
// (msSoilMoisture).
// Cluster attributes definitions in https://www.st.com/resource/en/user_manual/um2977-stm32wb-series-zigbee-cluster-library-api-stmicroelectronics.pdf.

//#define GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_CLUSTER_ID 1032
#define GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_CLUSTER_ID 1459

// Soil moisture value represented as an uint16.
#define GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_VALUE_1_ID 0x00
#define GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_VALUE_2_ID 0x01

// Required callbacks. ZBOSS will call these.
void garden_zcl_soil_moisture_init_server(void);
void garden_zcl_soil_moisture_init_client(void);

#define GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_CLUSTER_ID_SERVER_ROLE_INIT garden_zcl_soil_moisture_init_server
#define GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_CLUSTER_ID_CLIENT_ROLE_INIT garden_zcl_soil_moisture_init_client


#define GARDEN_ZB_ZCL_SOIL_MOISTURE_CLUSTER_REVISION_DEFAULT ((zb_uint16_t)0x0042)

// for the first sensor
#define ZB_SET_ATTR_DESCR_WITH_GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_VALUE_1_ID(data_ptr) \
  {                                                                              \
    GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_VALUE_1_ID,                                     \
        ZB_ZCL_ATTR_TYPE_U16,                                                    \
        ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,             \
        (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                      \
        (void*)data_ptr                                                          \
  }

// for the second sensor
#define ZB_SET_ATTR_DESCR_WITH_GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_VALUE_2_ID(data_ptr) \
  {                                                                              \
    GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_VALUE_2_ID,                                     \
        ZB_ZCL_ATTR_TYPE_U16,                                                    \
        ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING,             \
        (ZB_ZCL_NON_MANUFACTURER_SPECIFIC),                                      \
        (void*)data_ptr                                                          \
  }

#define GARDEN_ZB_ZCL_DECLARE_SOIL_MOISTURE_ATTRIB_LIST(attr_list, value1, value2)                   \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, GARDEN_ZB_ZCL_SOIL_MOISTURE) \
  ZB_ZCL_SET_ATTR_DESC(GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_VALUE_1_ID, (value1))                  \
  ZB_ZCL_SET_ATTR_DESC(GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_VALUE_2_ID, (value2))                  \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

//*
void garden_zcl_soil_moisture_init_server(void) {
  zb_zcl_add_cluster_handlers(GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_CLUSTER_ID,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              /*cluster_check_value=*/NULL,
                              /*cluster_write_attr_hook=*/NULL,
                              /*cluster_handler=*/NULL);
}
//*/
void garden_zcl_soil_moisture_init_client(void) {
  // Nothing.
}

#endif  // _GARDEN_ZB_SOIL_MOISTURE_DEFS_


