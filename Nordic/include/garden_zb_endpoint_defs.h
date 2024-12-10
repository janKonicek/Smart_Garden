#ifndef _GARDEN_ZB_H_
#define _GARDEN_ZB_H_

#include "garden_zb_soil_moisture_defs.h"
#include "garden_zb_analog_output_defs.h"

#define GARDEN_ZIGBEE_ENDPOINT 11
#define GARDEN_BASIC_MANUF_NAME "b-parasite"

#define GARDEN_ZB_DEVICE_ID 0x0008
#define GARDEN_ZB_DEVICE_VERSION 0
#define GARDEN_ZB_IN_CLUSTER_NUM 5
#define GARDEN_ZB_OUT_CLUSTER_NUM 0
#define GARDEN_ZB_CLUSTER_NUM (GARDEN_ZB_IN_CLUSTER_NUM + GARDEN_ZB_OUT_CLUSTER_NUM)
#define GARDEN_ZB_ATTR_REPORTING_COUNT 3

#define GARDEN_ZB_DECLARE_CLUSTER_LIST(                             \
    cluster_list_name,                                              \
    basic_attr_list,                                                \
    identify_attr_list,                                             \
    on_off_attr_list,                                               \
    analog_output_attr_list,                                          \
    soil_moisture_attr_list)                                        \
  zb_zcl_cluster_desc_t cluster_list_name[] =                               \
      {                                                                     \
          ZB_ZCL_CLUSTER_DESC(                                              \
              ZB_ZCL_CLUSTER_ID_IDENTIFY,                                   \
              ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),         \
              (identify_attr_list),                                         \
              ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
              ZB_ZCL_MANUF_CODE_INVALID),                                   \
          ZB_ZCL_CLUSTER_DESC(                                              \
              ZB_ZCL_CLUSTER_ID_BASIC,                                      \
              ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),            \
              (basic_attr_list),                                            \
              ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
              ZB_ZCL_MANUF_CODE_INVALID),                                   \
          ZB_ZCL_CLUSTER_DESC(                                              \
              ZB_ZCL_CLUSTER_ID_ON_OFF,                                     \
              ZB_ZCL_ARRAY_SIZE(on_off_attr_list, zb_zcl_attr_t),           \
              (on_off_attr_list),                                           \
              ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
              ZB_ZCL_MANUF_CODE_INVALID),                                   \
          ZB_ZCL_CLUSTER_DESC(                                              \
              GARDEN_ZB_ZCL_ATTR_ANALOG_OUTPUT_CLUSTER_ID,                  \
              ZB_ZCL_ARRAY_SIZE(analog_output_attr_list, zb_zcl_attr_t),           \
              (analog_output_attr_list),                                           \
              ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
              ZB_ZCL_MANUF_CODE_INVALID),                                   \
          ZB_ZCL_CLUSTER_DESC(                                              \
              GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_CLUSTER_ID,                  \
              ZB_ZCL_ARRAY_SIZE(soil_moisture_attr_list, zb_zcl_attr_t),    \
              (soil_moisture_attr_list),                                    \
              ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
              ZB_ZCL_MANUF_CODE_INVALID)                                    \
        }

        /* ZB_ZCL_CLUSTER_DESC(                                              \
             ZB_ZCL_CLUSTER_ID_ON_OFF,                    \
             ZB_ZCL_ARRAY_SIZE(on_off_attr_list, zb_zcl_attr_t),    \
             (on_off_attr_list),                                    \
             ZB_ZCL_CLUSTER_SERVER_ROLE,                                   \
             ZB_ZCL_MANUF_CODE_INVALID),*/

#define GARDEN_ZB_DECLARE_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
    ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                           \
    ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)                            \
    simple_desc_##ep_name =                                                        \
    {                                                                          \
        ep_id,                                                                 \
        ZB_AF_HA_PROFILE_ID,                                                   \
        GARDEN_ZB_DEVICE_ID,                                                   \
        GARDEN_ZB_DEVICE_VERSION,                                              \
        0,                                                                     \
        in_clust_num,                                                          \
        out_clust_num,                                                         \
        {                                                                         \
            ZB_ZCL_CLUSTER_ID_BASIC,                                              \
            ZB_ZCL_CLUSTER_ID_IDENTIFY,                                           \
            ZB_ZCL_CLUSTER_ID_ON_OFF,                                             \
            ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT,                                      \
            GARDEN_ZB_ZCL_ATTR_SOIL_MOISTURE_CLUSTER_ID,                          \
        }                                                                         \
    }

        /*ZB_ZCL_CLUSTER_ID_ON_OFF,*/
#define GARDEN_ZB_DECLARE_ENDPOINT(ep_name, ep_id, cluster_list)                                      \
            ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_ctx_##ep_name, GARDEN_ZB_ATTR_REPORTING_COUNT);      \
            GARDEN_ZB_DECLARE_SIMPLE_DESC(ep_name, ep_id,                                                     \
                                          GARDEN_ZB_IN_CLUSTER_NUM, GARDEN_ZB_OUT_CLUSTER_NUM);               \
            ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID,                                  \
                                        /*reserved_length=*/0, /*reserved_ptr=*/NULL,                         \
                                        ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), cluster_list, \
                                        (zb_af_simple_desc_1_1_t *)&simple_desc_##ep_name,                    \
                                        GARDEN_ZB_ATTR_REPORTING_COUNT, reporting_ctx_##ep_name,              \
                                        /*lev_ctrl_count=*/0, /*lev_ctrl_ctx=*/NULL)

#endif // _GARDEN_ZB_H_
