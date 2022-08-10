#ifndef __GREYBUS_TESTBENCH_H
#define __GREYBUS_TESTBENCH_H

#define __packed __attribute__((__packed__))

#include <stdint.h>

#ifndef BIT
#define BIT(N)  (1UL << (N))
#endif

#include "greybus_manifest.h"
#include "greybus_protocols.h"

/* Wouldn't support types larger than 4 bytes */
#define _ALIGNBYTES		(sizeof(uint32_t) - 1)
#define ALIGN(p)		((typeof(p))(((unsigned)(p) + _ALIGNBYTES) & ~_ALIGNBYTES))

#define GB_REQUEST_TYPE_PROTOCOL_VERSION 0x01
#define GREYBUS_VERSION_MAJOR	0x00
#define GREYBUS_VERSION_MINOR	0x01

/* Fixed values of AP's interface id and endo id */
#define ENDO_ID 0x4755
#define AP_INTF_ID 0x5

/* CPorts */

#define PROTOCOL_STATUS_SUCCESS	0x00
#define PROTOCOL_STATUS_INVALID	0x01
#define PROTOCOL_STATUS_NOMEM	0x02
#define PROTOCOL_STATUS_BUSY	0x03
#define PROTOCOL_STATUS_RETRY	0x04
#define PROTOCOL_STATUS_BAD	0xff

#define gb_operation_msg_size(hdr)	\
	le16toh(((struct gb_operation_msg_hdr *)(hdr))->size)


struct op_msg {
    struct gb_operation_msg_hdr			header;
    union {
        struct gb_control_version_response	control_version_rsp;
        struct gb_control_get_manifest_size_response control_msize_rsp;
        struct gb_control_get_manifest_response control_manifest_rsp;
        struct gb_control_bundle_pm_response	control_bundle_pm_rsp;
        struct gb_control_intf_pm_response	control_intf_pm_rsp;
        struct gb_svc_version_request		svc_version_request;
        struct gb_svc_version_response		svc_version_response;
        struct gb_svc_hello_request		hello_request;
        struct gb_svc_intf_device_id_request	svc_intf_device_id_request;
        struct gb_svc_conn_create_request	svc_conn_create_request;
        struct gb_svc_conn_destroy_request	svc_conn_destroy_request;
        struct gb_svc_module_inserted_request	svc_module_inserted_request;
        struct gb_svc_module_removed_request	svc_module_removed_request;
        struct gb_svc_intf_reset_request	svc_intf_reset_request;
        struct gb_svc_dme_peer_get_request	svc_dme_peer_get_request;
        struct gb_svc_dme_peer_get_response	svc_dme_peer_get_response;
        struct gb_svc_dme_peer_set_request	svc_dme_peer_set_request;
        struct gb_svc_dme_peer_set_response	svc_dme_peer_set_response;
        struct gb_svc_route_create_request	svc_route_create_request;
        struct gb_svc_route_destroy_request	svc_route_destroy_request;
        struct gb_svc_pwrmon_rail_count_get_response	svc_pwrmon_rail_count_get_response;
        struct gb_svc_intf_vsys_request		svc_intf_vsys_request;
        struct gb_svc_intf_vsys_response	svc_intf_vsys_response;
        struct gb_svc_intf_refclk_response	svc_intf_refclk_response;
        struct gb_svc_intf_unipro_response	svc_intf_unipro_response;
        struct gb_svc_intf_activate_response	svc_intf_activate_response;
        struct gb_svc_intf_resume_response	svc_intf_resume_response;
        struct gb_svc_intf_set_pwrm_response	svc_intf_set_pwrm_response;
        struct gb_gpio_line_count_response	gpio_lc_rsp;
        struct gb_gpio_activate_request		gpio_act_req;
        struct gb_gpio_deactivate_request	gpio_deact_req;
        struct gb_gpio_get_direction_request	gpio_get_dir_req;
        struct gb_gpio_get_direction_response	gpio_get_dir_rsp;
        struct gb_gpio_direction_in_request	gpio_dir_input_req;
        struct gb_gpio_direction_out_request	gpio_dir_output_req;
        struct gb_gpio_get_value_request	gpio_get_val_req;
        struct gb_gpio_get_value_response	gpio_get_val_rsp;
        struct gb_gpio_set_value_request	gpio_set_val_req;
        struct gb_gpio_set_debounce_request	gpio_set_db_req;
        struct gb_gpio_irq_type_request		gpio_irq_type_req;
        struct gb_gpio_irq_mask_request		gpio_irq_mask_req;
        struct gb_gpio_irq_unmask_request	gpio_irq_unmask_req;
        struct gb_gpio_irq_event_request	gpio_irq_event_req;
        struct gb_i2c_functionality_response	i2c_fcn_rsp;
        struct gb_i2c_transfer_request		i2c_xfer_req;
        struct gb_i2c_transfer_response		i2c_xfer_rsp;
        struct gb_pwm_count_response		pwm_cnt_rsp;
        struct gb_pwm_activate_request		pwm_act_req;
        struct gb_pwm_deactivate_request	pwm_deact_req;
        struct gb_pwm_config_request		pwm_cfg_req;
        struct gb_pwm_polarity_request		pwm_pol_req;
        struct gb_pwm_enable_request		pwm_enb_req;
        struct gb_pwm_disable_request		pwm_dis_req;
        struct gb_uart_send_data_request	uart_send_data_req;
        struct gb_uart_recv_data_request	uart_recv_data_rsp;
        struct gb_uart_set_break_request	uart_sb_req;
        struct gb_uart_serial_state_request	uart_ss_resp;
        struct gb_uart_set_line_coding_request	uart_slc_req;
        struct gb_uart_set_control_line_state_request uart_sls_req;
        struct gb_sdio_get_caps_response	sdio_caps_rsp;
        struct gb_sdio_event_request		sdio_event_req;
        struct gb_sdio_command_request		sdio_cmd_req;
        struct gb_sdio_command_response		sdio_cmd_rsp;
        struct gb_sdio_transfer_request		sdio_xfer_req;
        struct gb_sdio_transfer_response	sdio_xfer_rsp;
        struct gb_loopback_transfer_request	loopback_xfer_req;
        struct gb_loopback_transfer_response	loopback_xfer_resp;
        struct gb_bootrom_version_response	fw_version_response;
        struct gb_bootrom_firmware_size_request		fw_size_req;
        struct gb_bootrom_firmware_size_response	fw_size_resp;
        struct gb_bootrom_get_firmware_request	fw_get_firmware_req;
        struct gb_bootrom_get_firmware_response fw_get_firmware_resp;
        struct gb_bootrom_ready_to_boot_request fw_rbt_req;
        struct gb_lights_blink_request		lights_blink_req;
        struct gb_lights_get_lights_response	lights_gl_rsp;
        struct gb_lights_event_request		lights_gl_event_req;
        struct gb_lights_set_brightness_request	lights_glc_bright_req;
        struct gb_lights_set_fade_request	lights_glc_fade_req;
        struct gb_lights_set_color_request	lights_glc_color_req;
        struct gb_lights_blink_request		lights_glc_blink_req;
        struct gb_lights_get_light_config_request	lights_gl_conf_req;
        struct gb_lights_get_light_config_response	lights_gl_conf_rsp;
        struct gb_lights_get_channel_config_request	lights_glc_conf_req;
        struct gb_lights_get_channel_config_response	lights_glc_conf_rsp;
        struct gb_lights_get_channel_flash_config_request	lights_glc_fconf_req;
        struct gb_lights_get_channel_flash_config_response	lights_glc_fconf_rsp;
        struct gb_lights_set_flash_intensity_request	lights_glc_fint_req;
        struct gb_lights_set_flash_strobe_request	lights_glc_fstrobe_req;
        struct gb_lights_set_flash_timeout_request	lights_glc_ftimeout_req;
        struct gb_lights_get_flash_fault_request	lights_glc_ffault_req;
        struct gb_lights_get_flash_fault_response	lights_glc_ffault_rsp;
        struct gb_power_supply_get_supplies_response	psy_get_supplies_rsp;
        struct gb_power_supply_get_description_request	psy_get_desc_req;
        struct gb_power_supply_get_description_response	psy_get_desc_rsp;
        struct gb_power_supply_get_property_descriptors_request	psy_get_props_req;
        struct gb_power_supply_get_property_descriptors_response psy_get_props_rsp;
        struct gb_power_supply_get_property_request		psy_get_prop_req;
        struct gb_power_supply_get_property_response		psy_get_prop_rsp;
        struct gb_power_supply_set_property_request		psy_set_prop_req;
        struct gb_spi_master_config_response	spi_mc_rsp;
        struct gb_spi_device_config_request	spi_dc_req;
        struct gb_spi_device_config_response	spi_dc_rsp;
        struct gb_spi_transfer_request		spi_xfer_req;
        struct gb_spi_transfer_response		spi_xfer_rsp;
        struct gb_fw_download_find_firmware_request	fw_download_find_req;
        struct gb_fw_download_find_firmware_response	fw_download_find_rsp;
        struct gb_fw_download_fetch_firmware_request	fw_download_fetch_req;
        struct gb_fw_download_fetch_firmware_response	fw_download_fetch_rsp;
        struct gb_fw_download_release_firmware_request	fw_download_release_req;
        struct gb_fw_mgmt_interface_fw_version_response	fw_mgmt_intf_fw_version_rsp;
        struct gb_fw_mgmt_load_and_validate_fw_request	fw_mgmt_load_validate_fw_req;
        struct gb_fw_mgmt_loaded_fw_request		fw_mgmt_loaded_fw_req;
        struct gb_fw_mgmt_backend_fw_version_request	fw_mgmt_backend_fw_ver_req;
        struct gb_fw_mgmt_backend_fw_version_response	fw_mgmt_backend_fw_ver_rsp;
        struct gb_fw_mgmt_backend_fw_update_request	fw_mgmt_backend_fw_update_req;
        struct gb_fw_mgmt_backend_fw_updated_request	fw_mgmt_backend_fw_updated_req;
    };
};

#define OP_RESPONSE			0x80

int svc_init(void);
void svc_exit(void);

char *svc_get_operation(uint8_t type);
char *control_get_operation(uint8_t type);

int send_response(uint16_t hd_cport_id, struct op_msg *msg, uint16_t msg_size,
                  uint16_t operation_id, uint8_t type, uint8_t result);
int send_request(uint16_t hd_cport_id, struct op_msg *msg, uint16_t msg_size,
                 uint16_t operation_id, uint8_t type);

#endif //__GREYBUS_TESTBENCH_H
