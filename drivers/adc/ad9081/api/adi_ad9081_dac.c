/*!
 * @brief     APIs for DAC NCO configuration and control
 *
 * @copyright copyright(c) 2018 analog devices, inc. all rights reserved.
 *            This software is proprietary to Analog Devices, Inc. and its
 *            licensor. By using this software you agree to the terms of the
 *            associated analog devices software license agreement.
 */

/*!
 * @addtogroup __AD9081_DAC_API__
 * @{
 */

/*============= I N C L U D E S ============*/
#include "adi_ad9081_config.h"
#include "adi_ad9081_hal.h"

/*============= C O D E ====================*/
int32_t adi_ad9081_dac_select_set(adi_ad9081_device_t *device, uint8_t dacs)
{
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(dacs > AD9081_DAC_ALL);
	return adi_ad9081_hal_reg_set(device, REG_PAGEINDX_DAC_MAINDP_DAC_ADDR,
				      dacs); /* not paged */
}

int32_t adi_ad9081_dac_chan_select_set(adi_ad9081_device_t *device,
				       uint8_t channels)
{
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(channels > AD9081_DAC_CH_ALL);
	return adi_ad9081_hal_reg_set(device, REG_PAGEINDX_DAC_CHAN_ADDR,
				      channels); /* not paged */
}

int32_t adi_ad9081_dac_duc_select_set(adi_ad9081_device_t *device, uint8_t dacs,
				      uint8_t channels)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	err = adi_ad9081_dac_select_set(device, dacs);
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_dac_chan_select_set(device, channels);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_d2a_dual_spi_enable_set(adi_ad9081_device_t *device,
					       uint8_t duals, uint8_t enable)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	err = adi_ad9081_hal_bf_set(
		device, REG_SPI_ENABLE_DAC_ADDR, BF_SPI_EN_D2A0_INFO,
		((duals & 0x1) > 0 ? 0x1 : 0x0) & enable); /* not paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(
		device, REG_SPI_ENABLE_DAC_ADDR, BF_SPI_EN_D2A1_INFO,
		((duals & 0x2) > 0 ? 0x1 : 0x0) & enable); /* not paged */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_mode_switch_group_select_set(
	adi_ad9081_device_t *device,
	adi_ad9081_dac_mode_switch_group_select_e group)
{
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	return adi_ad9081_hal_bf_set(device, REG_PAGEINDX_DAC_JRX_ADDR,
				     BF_MODS_MSK_INFO, group);
}

int32_t
adi_ad9081_dac_mode_set(adi_ad9081_device_t *device,
			adi_ad9081_dac_mode_switch_group_select_e groups,
			adi_ad9081_dac_mode_e mode)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	if ((groups & AD9081_DAC_MODE_SWITCH_GROUP_0) > 0) {
		err = adi_ad9081_dac_mode_switch_group_select_set(
			device, AD9081_DAC_MODE_SWITCH_GROUP_0);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_hal_bf_set(device, REG_DDSM_DATAPATH_CFG_ADDR,
					    BF_DDSM_MODE_INFO,
					    mode); /* paged */
		AD9081_ERROR_RETURN(err);
	}
	if ((groups & AD9081_DAC_MODE_SWITCH_GROUP_1) > 0) {
		err = adi_ad9081_dac_mode_switch_group_select_set(
			device, AD9081_DAC_MODE_SWITCH_GROUP_1);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_hal_bf_set(device, REG_DDSM_DATAPATH_CFG_ADDR,
					    BF_DDSM_MODE_INFO,
					    mode); /* paged */
		AD9081_ERROR_RETURN(err);
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_complex_modulation_enable_set(
	adi_ad9081_device_t *device,
	adi_ad9081_dac_mode_switch_group_select_e groups, uint8_t enable)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	if ((groups & AD9081_DAC_MODE_SWITCH_GROUP_0) > 0) {
		err = adi_ad9081_dac_mode_switch_group_select_set(
			device, AD9081_DAC_MODE_SWITCH_GROUP_0);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_hal_bf_set(device, REG_DDSM_DATAPATH_CFG_ADDR,
					    BF_EN_CMPLX_MODULATION_INFO,
					    enable); /* paged */
		AD9081_ERROR_RETURN(err);
	}
	if ((groups & AD9081_DAC_MODE_SWITCH_GROUP_1) > 0) {
		err = adi_ad9081_dac_mode_switch_group_select_set(
			device, AD9081_DAC_MODE_SWITCH_GROUP_1);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_hal_bf_set(device, REG_DDSM_DATAPATH_CFG_ADDR,
					    BF_EN_CMPLX_MODULATION_INFO,
					    enable); /* paged */
		AD9081_ERROR_RETURN(err);
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_nco_gain_set(adi_ad9081_device_t *device,
					uint8_t channels, uint16_t gain)
{
	int32_t err;
	uint8_t i, channel;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(gain > 0x0FFF);

	for (i = 0; i < 8; i++) {
		channel = channels & (AD9081_DAC_CH_0 << i);
		if (channel > 0) {
			err = adi_ad9081_dac_chan_select_set(device, channel);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_CHNL_GAIN0_ADDR,
						    BF_CHNL_GAIN_INFO,
						    gain); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_nco_gains_set(adi_ad9081_device_t *device,
					 uint16_t gains[8])
{
	int32_t err;
	uint8_t i;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 8; i++) {
		err = adi_ad9081_dac_duc_nco_gain_set(
			device, AD9081_DAC_CH_0 << i, gains[i]);
		AD9081_ERROR_RETURN(err);
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_chan_skew_set(adi_ad9081_device_t *device,
					 uint8_t channels, uint8_t skew)
{
	int32_t err;
	uint8_t i, channel;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 8; i++) {
		channel = channels & (AD9081_DAC_CH_0 << i);
		if (channel > 0) {
			err = adi_ad9081_dac_chan_select_set(device, channel);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_CHNL_SKEW_ADJUST_ADDR,
						    BF_CHNL_SKEW_ADJ_INFO,
						    skew); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_nco_reset_set(adi_ad9081_device_t *device,
					 uint8_t chan_nco_reset,
					 uint8_t main_nco_reset)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	err = adi_ad9081_hal_bf_set(device, REG_CHNL_NCO_RST_EN_ADDR,
				    BF_SPI_CHNL_NCO_RST_EN_INFO,
				    chan_nco_reset); /* not paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_MAIN_NCO_RST_EN_ADDR,
				    BF_SPI_MAIN_NCO_RST_EN_INFO,
				    main_nco_reset); /* not paged */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_nco_enable_set(adi_ad9081_device_t *device,
					  uint8_t dacs, uint8_t channels,
					  uint8_t enable)
{
	int32_t err;
	uint8_t i, dac, channel;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(dacs > AD9081_DAC_ALL);

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSM_DATAPATH_CFG_ADDR,
						    BF_DDSM_NCO_EN_INFO,
						    enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	for (i = 0; i < 8; i++) {
		channel = channels & (AD9081_DAC_CH_0 << i);
		if (channel > 0) {
			err = adi_ad9081_dac_chan_select_set(device, channel);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSC_DATAPATH_CFG_ADDR,
						    BF_DDSC_NCO_EN_INFO,
						    enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_nco_phase_offset_set(adi_ad9081_device_t *device,
						uint8_t dacs,
						uint16_t dac_phase_offset,
						uint8_t channels,
						uint16_t ch_phase_offset)
{
	int32_t err;
	uint8_t i, dac, channel;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(dacs > AD9081_DAC_ALL);

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_DDSM_PHASE_OFFSET0_ADDR,
				BF_DDSM_NCO_PHASE_OFFSET_INFO,
				dac_phase_offset); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	for (i = 0; i < 8; i++) {
		channel = channels & (AD9081_DAC_CH_0 << i);
		if (channel > 0) {
			err = adi_ad9081_dac_chan_select_set(device, channel);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_DDSC_PHASE_OFFSET0_ADDR,
				BF_DDSC_NCO_PHASE_OFFSET_INFO,
				ch_phase_offset); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_nco_ftw0_set(adi_ad9081_device_t *device,
					uint8_t dacs, uint8_t channels,
					uint64_t ftw)
{
	int32_t err;
	uint8_t i, dac, channel, bit_val;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_DDSM_FTW0_ADDR,
						    BF_DDSM_FTW_INFO,
						    ftw); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSM_FTW_UPDATE_ADDR,
						    BF_DDSM_FTW_LOAD_REQ_INFO,
						    0); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSM_FTW_UPDATE_ADDR,
						    BF_DDSM_FTW_LOAD_REQ_INFO,
						    1); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_get(device,
						    REG_DDSM_FTW_UPDATE_ADDR,
						    0x00000101, &bit_val,
						    1); /* paged */
			AD9081_ERROR_RETURN(err);
			if (bit_val == 0) {
				AD9081_LOG_WARN("main ftw is not updated.");
			}
		}
	}

	for (i = 0; i < 8; i++) {
		channel = channels & (AD9081_DAC_CH_0 << i);
		if (channel > 0) {
			err = adi_ad9081_dac_chan_select_set(device, channel);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_DDSC_FTW0_ADDR,
						    BF_DDSC_FTW_INFO,
						    ftw); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSC_FTW_UPDATE_ADDR,
						    BF_DDSC_FTW_LOAD_REQ_INFO,
						    0); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSC_FTW_UPDATE_ADDR,
						    BF_DDSC_FTW_LOAD_REQ_INFO,
						    1); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_get(device,
						    REG_DDSC_FTW_UPDATE_ADDR,
						    0x00000101, &bit_val,
						    1); /* paged */
			AD9081_ERROR_RETURN(err);
			if (bit_val == 0) {
				AD9081_LOG_WARN("channel ftw is not updated.");
			}
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_nco_ftw_set(adi_ad9081_device_t *device,
				       uint8_t dacs, uint8_t channels,
				       uint64_t ftw, uint64_t acc_modulus,
				       uint64_t acc_delta)
{
	int32_t err;
	uint8_t i, dac, channel;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_DDSM_FTW_UPDATE_ADDR,
				BF_DDSM_FTW_LOAD_SYSREF_INFO, 0); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSM_FTW_UPDATE_ADDR,
						    0x00000304, 0); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_DDSM_DATAPATH_CFG_ADDR,
				BF_DDSM_MODULUS_EN_INFO,
				(acc_modulus > 0 ? 1 : 0)); /* paged */
			AD9081_ERROR_RETURN(err);
			if (acc_modulus > 0) {
				err = adi_ad9081_hal_bf_set(
					device, REG_DDSM_ACC_MODULUS0_ADDR,
					BF_DDSM_ACC_MODULUS_INFO,
					acc_modulus); /* paged */
				AD9081_ERROR_RETURN(err);
				err = adi_ad9081_hal_bf_set(
					device, REG_DDSM_ACC_DELTA0_ADDR,
					BF_DDSM_ACC_DELTA_INFO,
					acc_delta); /* paged */
				AD9081_ERROR_RETURN(err);
			}
			err = adi_ad9081_dac_duc_nco_ftw0_set(
				device, dacs, AD9081_DAC_CH_NONE, ftw);
			AD9081_ERROR_RETURN(err);
		}
	}

	for (i = 0; i < 8; i++) {
		channel = channels & (AD9081_DAC_CH_0 << i);
		if (channel > 0) {
			err = adi_ad9081_dac_chan_select_set(device, channel);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_DDSC_FTW_UPDATE_ADDR,
				BF_DDSC_FTW_LOAD_SYSREF_INFO, 0); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSC_FTW_UPDATE_ADDR,
						    0x00000304, 0); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_DDSC_DATAPATH_CFG_ADDR,
				BF_DDSC_MODULUS_EN_INFO,
				(acc_modulus > 0 ? 1 : 0)); /* paged */
			AD9081_ERROR_RETURN(err);
			if (acc_modulus > 0) {
				err = adi_ad9081_hal_bf_set(
					device, REG_DDSC_ACC_MODULUS0_ADDR,
					BF_DDSC_ACC_MODULUS_INFO,
					acc_modulus); /* paged */
				AD9081_ERROR_RETURN(err);
				err = adi_ad9081_hal_bf_set(
					device, REG_DDSC_ACC_DELTA0_ADDR,
					BF_DDSC_ACC_DELTA_INFO,
					acc_delta); /* paged */
				AD9081_ERROR_RETURN(err);
			}
			err = adi_ad9081_dac_duc_nco_ftw0_set(
				device, AD9081_DAC_NONE, channel, ftw);
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_nco_set(adi_ad9081_device_t *device, uint8_t dacs,
				   uint8_t channels, int64_t nco_shift_hz)
{
	int32_t err;
	uint64_t ftw;
	uint8_t main_interp = 0;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(device->dev_info.dac_freq_hz == 0);

	/* set main nco */
	if (dacs != AD9081_DAC_NONE) {
		err = adi_ad9081_dac_duc_nco_enable_set(device, dacs,
							AD9081_DAC_CH_NONE, 1);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_hal_calc_tx_nco_ftw(
			device, device->dev_info.dac_freq_hz, nco_shift_hz,
			&ftw);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_dac_duc_nco_ftw_set(
			device, dacs, AD9081_DAC_CH_NONE, ftw, 0, 0);
		AD9081_ERROR_RETURN(err);
	}

	/* set channel nco */
	if (channels != AD9081_DAC_CH_NONE) {
		err = adi_ad9081_hal_bf_get(device, REG_INTRP_MODE_ADDR,
					    BF_DP_INTERP_MODE_INFO,
					    &main_interp, 1);
		AD9081_ERROR_RETURN(err);

		err = adi_ad9081_dac_duc_nco_enable_set(device, AD9081_DAC_NONE,
							channels, 1);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_hal_calc_tx_nco_ftw(
			device, device->dev_info.dac_freq_hz,
			nco_shift_hz * main_interp, &ftw);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_dac_duc_nco_ftw_set(device, AD9081_DAC_NONE,
						     channels, ftw, 0, 0);
		AD9081_ERROR_RETURN(err);
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_main_nco_hopf_ftw_set(adi_ad9081_device_t *device,
						 uint8_t dacs,
						 uint8_t hopf_index,
						 uint32_t hopf_ftw)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(hopf_index > 31);
	AD9081_INVALID_PARAM_RETURN(hopf_index < 1);

	/* set ftw1 ~ ftw31 */
	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device,
				REG_DDSM_HOPF_FTW1_0_ADDR +
					4 * (hopf_index - 1),
				BF_DDSM_HOPF_FTW1_INFO, hopf_ftw); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_main_nco_hopf_mode_set(adi_ad9081_device_t *device,
						  uint8_t dacs,
						  uint8_t hopf_mode)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			/* 0: phase continuous switch, 1: phase in-continuous switch, 2: phase coherent switch */
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSM_HOPF_CTRL0_ADDR,
						    BF_DDSM_HOPF_MODE_INFO,
						    hopf_mode); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_main_nco_hopf_select_set(adi_ad9081_device_t *device,
						    uint8_t dacs,
						    uint8_t hopf_index)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(hopf_index > 31);

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSM_HOPF_CTRL0_ADDR,
						    BF_DDSM_HOPF_SEL_INFO,
						    hopf_index); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_main_nco_hopf_gpio_no_glitch_en_set(
	adi_ad9081_device_t *device, uint8_t dacs, uint8_t enable)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(enable > 1);

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSM_HOPF_CTRL0_ADDR,
						    0x00000105,
						    enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t
adi_ad9081_dac_duc_main_nco_hopf_gpio_as_hop_en_set(adi_ad9081_device_t *device,
						    uint8_t enable)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(enable > 1);

	if (enable > 0) {
		err = adi_ad9081_hal_reg_set(
			device, REG_GPIO_CFG3_ADDR,
			0x13); /* dac_nco_ffh1.dac_nco_ffh0 */
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_hal_reg_set(
			device, REG_GPIO_CFG0_ADDR,
			0x33); /* dac_nco_ffh3.dac_nco_ffh2 */
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_hal_reg_set(
			device, REG_GPIO_CFG1_ADDR,
			0x33); /* dac_nco_ffh5.dac_nco_ffh4 */
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_hal_reg_set(
			device, REG_GPIO_CFG2_ADDR,
			0x22); /* dac_nco_ffh_strobe.dac_nco_ffh6 */
		AD9081_ERROR_RETURN(err);
	}
	err = adi_ad9081_hal_bf_set(device, REG_DDSM_HOPF_CTRL1_ADDR,
				    BF_GPIO_HOP_EN_INFO,
				    enable); /* not paged */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_gpio_as_sync1_out_set(adi_ad9081_device_t *device,
					     uint8_t mode)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(mode > 1);

	/* 0: link1_sync, 1: link1_sync with diff mode */
	err = adi_ad9081_hal_bf_set(device, REG_GPIO_CFG3_ADDR,
				    BF_SYNC1_OUTBP_CFG_INFO,
				    mode == 0 ? 1 : 9); /* link1_sync */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_gpio_as_pa_en_set(adi_ad9081_device_t *device,
					 uint8_t enable)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(enable > 1);

	if (enable > 0) {
		err = adi_ad9081_hal_reg_set(device, REG_GPIO_CFG0_ADDR,
					     0x11); /* pa1_en.pa0_en */
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_hal_reg_set(device, REG_GPIO_CFG1_ADDR,
					     0x21); /* pa3_en.pa2_en */
		AD9081_ERROR_RETURN(err);
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_gpio_as_tx_en_set(adi_ad9081_device_t *device,
					 uint8_t enable)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(enable > 1);

	if (enable > 0) {
		err = adi_ad9081_hal_reg_set(device, REG_GPIO_CFG2_ADDR,
					     0x11); /* txen3.txen1 */
		AD9081_ERROR_RETURN(err);
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_interpolation_set(adi_ad9081_device_t *device,
					 uint8_t main_interp, uint8_t ch_interp)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	err = adi_ad9081_hal_reg_set(device, REG_INTRP_MODE_ADDR,
				     (main_interp << 4) +
					     ch_interp); /* not paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_MAINDP_DAC_1XXX_ENABLES_ADDR,
				    BF_MAINDP_DAC_1XXX_EN_SPI_INFO,
				    (ch_interp > 1) ? 0 : 1); /* not paged */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_spi_enable_set(adi_ad9081_device_t *device,
				      uint8_t enable)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	err = adi_ad9081_hal_reg_set(device, REG_SPI_ENABLE_DAC_ADDR,
				     (enable > 0) ? 0x1f :
						    0x00); /* not paged */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_digital_logic_enable_set(adi_ad9081_device_t *device,
						uint8_t enable)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* enable digital logic, including jrx digital, digital clock gen., digital data path */
	err = adi_ad9081_hal_bf_set(device, REG_DIG_RESET_ADDR,
				    BF_DIG_RESET_INFO, !enable); /* not paged */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_dll_startup(adi_ad9081_device_t *device, uint8_t dacs)
{
	int32_t err;
	uint8_t bf_val;
	uint8_t clk_on_val = 2, clk_off_val = 1;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* setup clocks */
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(
		device, 0x3, 1); /* select dual0 & dual1 */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_GENERAL_ADDR,
				    BF_DLLCLK_ENCTRL_INFO,
				    clk_on_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(device, 0x1,
						     1); /* select dual0 */
	AD9081_ERROR_RETURN(err);
	bf_val = (dacs & AD9081_DAC_0) > 0 ? clk_on_val : clk_off_val;
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC0_ADDR,
				    BF_MUSHICLKEN_CTRL_DAC0_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC0_ADDR,
				    BF_FORCEHIGH_CTRL_DAC0_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC0_ADDR,
				    BF_SWDCLKEN_CTRL_DAC0_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	bf_val = (dacs & AD9081_DAC_1) > 0 ? clk_on_val : clk_off_val;
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC1_ADDR,
				    BF_MUSHICLKEN_CTRL_DAC1_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC1_ADDR,
				    BF_FORCEHIGH_CTRL_DAC1_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC1_ADDR,
				    BF_SWDCLKEN_CTRL_DAC1_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(device, 0x2,
						     1); /* select dual1 */
	AD9081_ERROR_RETURN(err);
	bf_val = (dacs & AD9081_DAC_3) > 0 ? clk_on_val : clk_off_val;
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC0_ADDR,
				    BF_MUSHICLKEN_CTRL_DAC0_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC0_ADDR,
				    BF_FORCEHIGH_CTRL_DAC0_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC0_ADDR,
				    BF_SWDCLKEN_CTRL_DAC0_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	bf_val = (dacs & AD9081_DAC_2) > 0 ? clk_on_val : clk_off_val;
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC1_ADDR,
				    BF_MUSHICLKEN_CTRL_DAC1_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC1_ADDR,
				    BF_FORCEHIGH_CTRL_DAC1_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_ENABLE_TIMING_CTRL_DAC1_ADDR,
				    BF_SWDCLKEN_CTRL_DAC1_INFO,
				    bf_val); /* paged */
	AD9081_ERROR_RETURN(err);

	/* setup mushi */
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(device, 0x1,
						     1); /* select dual0 */
	AD9081_ERROR_RETURN(err);
	bf_val = (dacs & AD9081_DAC_0) > 0 ? 0 : 1;
	err = adi_ad9081_hal_bf_set(device, REG_MUSHI_CTRL_ADDR,
				    BF_MUSHI_PD0_DUM_INFO, bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_MUSHI_CTRL_ADDR,
				    BF_MUSHI_PD0_INFO, bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	bf_val = (dacs & AD9081_DAC_1) > 0 ? 0 : 1;
	err = adi_ad9081_hal_bf_set(device, REG_MUSHI_CTRL_ADDR,
				    BF_MUSHI_PD1_DUM_INFO, bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_MUSHI_CTRL_ADDR,
				    BF_MUSHI_PD1_INFO, bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(device, 0x2,
						     1); /* select dual1 */
	AD9081_ERROR_RETURN(err);
	bf_val = (dacs & AD9081_DAC_3) > 0 ? 0 : 1;
	err = adi_ad9081_hal_bf_set(device, REG_MUSHI_CTRL_ADDR,
				    BF_MUSHI_PD0_DUM_INFO, bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_MUSHI_CTRL_ADDR,
				    BF_MUSHI_PD0_INFO, bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	bf_val = (dacs & AD9081_DAC_2) > 0 ? 0 : 1;
	err = adi_ad9081_hal_bf_set(device, REG_MUSHI_CTRL_ADDR,
				    BF_MUSHI_PD1_DUM_INFO, bf_val); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_MUSHI_CTRL_ADDR,
				    BF_MUSHI_PD1_INFO, bf_val); /* paged */
	AD9081_ERROR_RETURN(err);

	/* override dcc code */
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(
		device, 0x3, 1); /* select dual0 & dual1 */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, 0x00000192, 0x00000600, 0x00);
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, 0x00000192, 0x00000107, 0x01);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_soft_off_gain_enable_set(adi_ad9081_device_t *device,
						uint8_t dacs, uint8_t enable)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_BE_SOFT_OFF_GAIN_CTRL_ADDR,
				BF_BE_SOFT_OFF_GAIN_EN_INFO,
				enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_soft_off_new_gain_enable_set(adi_ad9081_device_t *device,
						    uint8_t dacs,
						    uint8_t enable)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_BE_SOFT_OFF_GAIN_CTRL_ADDR,
				0x00000103, enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_soft_off_gain_ramp_rate_set(adi_ad9081_device_t *device,
						   uint8_t dacs, uint8_t rate)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_BE_SOFT_OFF_GAIN_CTRL_ADDR,
				BF_BE_GAIN_RAMP_RATE_INFO, rate); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_soft_off_enable_set(adi_ad9081_device_t *device,
					   uint8_t dacs, uint16_t enable)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_BE_SOFT_OFF_ENABLE0_ADDR, 0x1000,
				enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_soft_on_enable_set(adi_ad9081_device_t *device,
					  uint8_t dacs, uint8_t enable)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_BE_SOFT_ON_ENABLE_ADDR,
						    0x0800, enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_long_pa_set(adi_ad9081_device_t *device, uint8_t dacs,
				   uint8_t enable, uint8_t averaging_time,
				   uint16_t average_threshold)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_LONG_PA_CONTROL_ADDR,
						    BF_LONG_PA_AVG_TIME_INFO,
						    averaging_time); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_LONG_PA_THRES_LSB_ADDR,
				BF_LONG_PA_THRESHOLD_INFO,
				average_threshold); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_LONG_PA_CONTROL_ADDR,
						    BF_LONG_PA_ENABLE_INFO,
						    enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_long_pa_power_get(adi_ad9081_device_t *device,
					 uint8_t dacs, uint16_t *power)
{
	int32_t err;
	uint8_t i;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		if ((dacs & (AD9081_DAC_0 << i)) > 0) {
			err = adi_ad9081_dac_select_set(
				device, (dacs & (AD9081_DAC_0 << i)));
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_get(
				device, REG_LONG_PA_POWER_LSB_ADDR,
				BF_LONG_PA_POWER_INFO, (uint8_t *)power,
				sizeof(uint16_t)); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_short_pa_set(adi_ad9081_device_t *device, uint8_t dacs,
				    uint8_t enable, uint8_t averaging_time,
				    uint16_t average_threshold)
{
	int32_t err;
	uint8_t i;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		if ((dacs & (AD9081_DAC_0 << i)) > 0) {
			err = adi_ad9081_dac_select_set(
				device, (dacs & (AD9081_DAC_0 << i)));
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_SHORT_PA_CONTROL_ADDR,
						    BF_SHORT_PA_AVG_TIME_INFO,
						    averaging_time); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_SHORT_PA_THRES_LSB_ADDR,
				BF_SHORT_PA_THRESHOLD_INFO,
				average_threshold); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_SHORT_PA_CONTROL_ADDR,
						    BF_SHORT_PA_ENABLE_INFO,
						    enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_short_pa_power_get(adi_ad9081_device_t *device,
					  uint8_t dacs, uint16_t *power)
{
	int32_t err;
	uint8_t i;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		if ((dacs & (AD9081_DAC_0 << i)) > 0) {
			err = adi_ad9081_dac_select_set(
				device, (dacs & (AD9081_DAC_0 << i)));
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_get(
				device, REG_SHORT_PA_POWER_LSB_ADDR,
				BF_SHORT_PA_POWER_INFO, (uint8_t *)power,
				sizeof(uint16_t)); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_rotation_mode_set(adi_ad9081_device_t *device,
					 uint8_t mode)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* bit0: enable jesd auto off/on during rotation, bit1: enable data path auto soft off/on during rotation */
	err = adi_ad9081_hal_bf_set(device, REG_ROTATION_MODE_ADDR,
				    BF_ROTATION_MODE_INFO,
				    mode); /* not paged */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_xbar_set(adi_ad9081_device_t *device, uint8_t dacs,
				uint8_t channel)
{
	int32_t err;
	uint8_t ch_interp;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(dacs > AD9081_DAC_ALL);

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_get(device, REG_INTRP_MODE_ADDR,
						    BF_CH_INTERP_MODE_INFO,
						    &ch_interp,
						    1); /* not paged */
			AD9081_ERROR_RETURN(err);
			if (ch_interp > 1) {
				err = adi_ad9081_hal_bf_set(
					device, 0x000001BA, 0x00000800,
					channel); /* paged */
				AD9081_ERROR_RETURN(err);
			} else { /* crossbar for 1xNx mode */
				err = adi_ad9081_hal_bf_set(
					device,
					REG_MAINDP_DAC_1XXX_ENABLES_ADDR,
					BF_MAINDP_DAC_1XXX_ENABLES_INFO,
					channel); /* paged */
				AD9081_ERROR_RETURN(err);
			}
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_irqs_enable_set(adi_ad9081_device_t *device,
				       uint64_t enable)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* enable irqs */
	err = adi_ad9081_hal_bf_set(device, REG_IRQ_ENABLE_0_ADDR, 0x2800,
				    enable); /* 5 8bit regs */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_irqs_status_get(adi_ad9081_device_t *device,
				       uint64_t *status)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* get irqs status */
	err = adi_ad9081_hal_bf_get(device, REG_IRQ_STATUS0_ADDR, 0x2800,
				    (uint8_t *)status, sizeof(uint64_t));
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_irqs_status_clr(adi_ad9081_device_t *device,
				       uint64_t clear)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* clr irqs status */
	err = adi_ad9081_hal_bf_set(device, REG_IRQ_STATUS0_ADDR, 0x2800,
				    clear);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_shuffle_enable_set(adi_ad9081_device_t *device,
					  uint8_t dacs, uint8_t enable)
{
	int32_t err;
	uint8_t duals = 0;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* DAC0/1 in dual0, DAC2/3 in dual1 */
	duals |= ((dacs & AD9081_DAC_0) > 0 ? 0x1 : 0x0);
	duals |= ((dacs & AD9081_DAC_1) > 0 ? 0x1 : 0x0);
	duals |= ((dacs & AD9081_DAC_2) > 0 ? 0x2 : 0x0);
	duals |= ((dacs & AD9081_DAC_3) > 0 ? 0x2 : 0x0);
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(device, duals, 1);
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_DECODE_MODE_ADDR,
				    BF_MSB_MODE_INFO, enable); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_DECODE_MODE_ADDR,
				    BF_ISB_MODE_INFO, enable); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(device, 0x3, 1);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_data_xor_set(adi_ad9081_device_t *device, uint8_t dacs,
				    uint8_t enable)
{
	int32_t err;
	uint8_t xor_en, dexor_en;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	xor_en = dacs & (enable > 0 ? 0xf : 0x0);
	err = adi_ad9081_hal_bf_set(device, REG_RETIMER_DEBUG0_ADDR,
				    BF_DAC_DATA_XOR_EN_INFO,
				    xor_en); /* not paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(device, 0x1,
						     1); /* select dual0 */
	AD9081_ERROR_RETURN(err);
	dexor_en = ((dacs & AD9081_DAC_0) > 0 ? 0x1 : 0x0) &
		   (enable > 0 ? 0x1 : 0x0);
	err = adi_ad9081_hal_bf_set(device, REG_HANDOFF_DEBUG_ADDR,
				    BF_DATA_DEXOR_EN_DAC0_INFO,
				    dexor_en); /* paged */
	AD9081_ERROR_RETURN(err);
	dexor_en = ((dacs & AD9081_DAC_1) > 0 ? 0x1 : 0x0) &
		   (enable > 0 ? 0x1 : 0x0);
	err = adi_ad9081_hal_bf_set(device, REG_HANDOFF_DEBUG_ADDR,
				    BF_DATA_DEXOR_EN_DAC1_INFO,
				    dexor_en); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(device, 0x2,
						     1); /* select dual1 */
	AD9081_ERROR_RETURN(err);
	dexor_en = ((dacs & AD9081_DAC_3) > 0 ? 0x1 : 0x0) &
		   (enable > 0 ? 0x1 : 0x0);
	err = adi_ad9081_hal_bf_set(device, REG_HANDOFF_DEBUG_ADDR,
				    BF_DATA_DEXOR_EN_DAC0_INFO,
				    dexor_en); /* paged */
	AD9081_ERROR_RETURN(err);
	dexor_en = ((dacs & AD9081_DAC_2) > 0 ? 0x1 : 0x0) &
		   (enable > 0 ? 0x1 : 0x0);
	err = adi_ad9081_hal_bf_set(device, REG_HANDOFF_DEBUG_ADDR,
				    BF_DATA_DEXOR_EN_DAC1_INFO,
				    dexor_en); /* paged */
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_dac_d2a_dual_spi_enable_set(device, 0x3, 1);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t
adi_ad9081_dac_tx_enable_state_machine_enable_set(adi_ad9081_device_t *device,
						  uint8_t dacs, uint8_t enable)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			/* setup txen state machine */
			err = adi_ad9081_hal_bf_set(device, REG_TXEN_SM_0_ADDR,
						    BF_ENA_TXENSM_INFO,
						    enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_spi_as_tx_en_set(adi_ad9081_device_t *device,
					uint8_t dacs, uint8_t enable)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			/* set txen enable */
			err = adi_ad9081_hal_bf_set(device,
						    REG_BLANKING_CTRL_ADDR,
						    BF_ENA_SPI_TXEN_INFO,
						    enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_tx_enable_set(adi_ad9081_device_t *device, uint8_t dacs,
				     uint8_t enable)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_spi_as_tx_en_set(device, dacs, 1);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			/* set txen */
			err = adi_ad9081_hal_bf_set(device,
						    REG_BLANKING_CTRL_ADDR,
						    BF_SPI_TXEN_INFO,
						    enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_power_up_set(adi_ad9081_device_t *device, uint8_t dacs,
				    uint8_t enable)
{
	int32_t err;
	uint8_t tmp_reg = 0x00;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(dacs > AD9081_DAC_ALL);
	AD9081_INVALID_PARAM_RETURN(enable > 1);

	/* power up dacs */
	err = adi_ad9081_hal_reg_get(device, (uint16_t)REG_DAC_POWERDOWN_ADDR,
				     &tmp_reg);
	AD9081_ERROR_RETURN(err);
	if (enable) {
		tmp_reg &= (~(uint8_t)dacs);
	} else {
		tmp_reg |= (uint8_t)dacs;
	}
	err = adi_ad9081_hal_reg_set(device, (uint16_t)REG_DAC_POWERDOWN_ADDR,
				     tmp_reg);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_nco_set(adi_ad9081_device_t *device, uint8_t dacs,
			       uint8_t channels, int64_t shift_hz,
			       uint16_t offset, uint8_t test_tone_en)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* set nco shift */
	err = adi_ad9081_dac_duc_nco_set(device, dacs, channels, shift_hz);
	AD9081_ERROR_RETURN(err);

	/* set test tone amplitude */
	if (dacs != AD9081_DAC_NONE) {
		err = adi_ad9081_dac_duc_main_dc_test_tone_en_set(device, dacs,
								  test_tone_en);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_dac_duc_main_dc_test_tone_offset_set(
			device, dacs, offset);
		AD9081_ERROR_RETURN(err);
	}
	if (channels != AD9081_DAC_CH_NONE) {
		err = adi_ad9081_dac_dc_test_tone_en_set(device, channels,
							 test_tone_en);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_dac_dc_test_tone_offset_set(device, channels,
							     offset);
		AD9081_ERROR_RETURN(err);
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_dc_test_tone_en_set(adi_ad9081_device_t *device,
					   uint8_t channels, uint8_t enable)
{
	int32_t err;
	uint8_t i, channel;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(enable > 1);

	for (i = 0; i < 8; i++) {
		channel = channels & (AD9081_DAC_CH_0 << i);
		if (channel > 0) {
			err = adi_ad9081_dac_chan_select_set(device, channel);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device,
						    REG_DDSC_DATAPATH_CFG_ADDR,
						    BF_TEST_TONE_EN_INFO,
						    enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_dc_test_tone_offset_set(adi_ad9081_device_t *device,
					       uint8_t channels,
					       uint16_t offset)
{
	int32_t err;
	uint8_t i, channel;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 8; i++) {
		channel = channels & (AD9081_DAC_CH_0 << i);
		if (channel > 0) {
			err = adi_ad9081_dac_chan_select_set(device, channel);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_DC_OFFSET0_ADDR,
						    BF_DC_OFFSET_INFO,
						    offset); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_main_dc_test_tone_en_set(adi_ad9081_device_t *device,
						    uint8_t dacs,
						    uint8_t enable)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(
				device, REG_DDSM_CAL_MODE_DEF_ADDR,
				BF_DDSM_EN_CAL_DC_INPUT_INFO,
				enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t
adi_ad9081_dac_duc_main_dc_test_tone_offset_set(adi_ad9081_device_t *device,
						uint8_t dacs, uint16_t offset)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, 0x000001E0,
						    0x00001000,
						    offset); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_main_dsa_enable_set(adi_ad9081_device_t *device,
					       uint8_t dacs, uint8_t enable)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_DSA_CFG2_ADDR,
						    BF_EN_DSA_CTRL_INFO,
						    enable); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_duc_main_dsa_set(adi_ad9081_device_t *device,
					uint8_t dacs, uint8_t code,
					uint8_t cutover, uint8_t boost,
					uint16_t gain)
{
	int32_t err;
	uint8_t i, dac;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_DSA_CFG0_ADDR,
						    BF_DSA_CTRL_INFO,
						    code); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_DSA_CFG1_ADDR,
						    BF_DSA_CUTOVER_INFO,
						    cutover); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_DSA_CFG2_ADDR,
						    BF_DSA_BOOST_INFO,
						    boost); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_DP_GAIN0_ADDR,
						    BF_DP_GAIN_INFO,
						    gain); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_DP_GAIN1_ADDR,
						    BF_GAIN_LOAD_STROBE_INFO,
						    1); /* paged */
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_DP_GAIN1_ADDR,
						    BF_GAIN_LOAD_STROBE_INFO,
						    0); /* paged */
			AD9081_ERROR_RETURN(err);
		}
	}

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_fsc_set(adi_ad9081_device_t *device, uint8_t dacs,
			       uint32_t uA)
{
	int32_t err;
	uint8_t i, dac, min_code;
	uint16_t c, f, fsc_code;
	uint32_t min_fsc, cf, ff;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	if (uA > 40000)
		AD9081_LOG_WARN(
			"full-scale current beyond design recommendation (7.5mA - 40mA)");
	if (uA < 7500)
		AD9081_LOG_WARN(
			"full-scale current beyond design recommendation (7.5mA - 40mA)");

	for (i = 0; i < 4; i++) {
		dac = dacs & (AD9081_DAC_0 << i);
		if (dac > 0) {
			err = adi_ad9081_dac_select_set(device, dac);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_get(device, REG_FSC0_ADDR,
						    BF_FSC_MIN_CTRL_INFO,
						    &min_code, 1);
			AD9081_ERROR_RETURN(err);

			min_fsc = (min_code * 25000) >> 4;
			if ((min_fsc + 24975) <
			    uA) { /* 24975 = 25.0 * 1023 / 1024 */
				while (((min_fsc + 24975) < uA) &&
				       (min_fsc < 15625)) {
					min_code = (min_code < 10) ?
							   (min_code + 1) :
							   10;
					min_fsc = (min_code * 25000) >> 4;
				}
			} else {
				while (((min_fsc + 5000) > uA) &&
				       (min_fsc > 1600)) {
					min_code = (min_code > 1) ?
							   (min_code - 1) :
							   1;
					min_fsc = (min_code * 25000) >> 4;
				}
			}
			err = adi_ad9081_hal_bf_set(device, REG_FSC0_ADDR,
						    BF_FSC_MIN_CTRL_INFO,
						    min_code);
			AD9081_ERROR_RETURN(err);
			c = (uint16_t)((((uA - min_fsc) << 10) + 25000 + 1) /
				       25000);
			f = (uint16_t)((((uA - min_fsc) << 10) / 25000));
			cf = min_fsc + ((c * 25000) >> 10);
			ff = min_fsc + ((f * 25000) >> 10);
			cf = cf > uA ? (cf - uA) : (uA - cf);
			ff = ff > uA ? (ff - uA) : (uA - ff);
			fsc_code = (cf < ff) ? c : f;
			fsc_code = (fsc_code < 0x3ff) ? fsc_code : 0x3ff;
			err = adi_ad9081_hal_bf_set(device, REG_FSC0_ADDR,
						    0x0200, fsc_code & 3);
			AD9081_ERROR_RETURN(err);
			err = adi_ad9081_hal_bf_set(device, REG_FSC1_ADDR,
						    0x0800, fsc_code >> 2);
			AD9081_ERROR_RETURN(err);
		}
	}
	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_sysref_phase_set(adi_ad9081_device_t *device,
					uint16_t phase)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* phase of measured sysref event, in dac clock unit */
	err = adi_ad9081_hal_bf_set(device, REG_SYSREF_PHASE0_ADDR,
				    BF_SYSREF_PHASE_INFO,
				    phase); /* not paged */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_sysref_sample_type_set(adi_ad9081_device_t *device,
					      uint8_t sample_type)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* 0 – sampled by reference clock then by high speed clock. 1 – sampled directly by high speed clock */
	err = adi_ad9081_hal_bf_set(device, REG_SYSREF_CTRL2_ADDR,
				    BF_SYSREF_SAMPLE_TYPE_INFO,
				    sample_type); /* not paged */
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_nco_reset_set(adi_ad9081_device_t *device)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	err = adi_ad9081_hal_bf_set(device, REG_DATAPATH_NCO_SYNC_CFG_ADDR,
				    BF_START_NCO_SYNC_INFO, 1);
	AD9081_ERROR_RETURN(err);
	err = adi_ad9081_hal_bf_set(device, REG_DATAPATH_NCO_SYNC_CFG_ADDR,
				    BF_START_NCO_SYNC_INFO, 0);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t
adi_ad9081_dac_nco_sync_reset_via_sysref_set(adi_ad9081_device_t *device,
					     uint8_t enable)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	err = adi_ad9081_hal_bf_set(device, 0x00000205, 0x00000102, enable);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_nco_sync_sysref_mode_set(adi_ad9081_device_t *device,
						uint8_t mode)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* 0: immediately by sysref, 1: by next lmfc rising edge, 2: by next lmfc falling edge */
	err = adi_ad9081_hal_bf_set(device, REG_NCOSYNC_SYSREF_MODE_ADDR,
				    BF_NCO_SYNC_SYSREF_MODE_INFO, mode);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_nco_master_slave_mode_set(adi_ad9081_device_t *device,
						 uint8_t mode)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* 0: disable, 1: master, 2: slave */
	err = adi_ad9081_hal_bf_set(device, REG_NCOSYNC_MS_MODE_ADDR,
				    BF_NCO_SYNC_MS_MODE_INFO, mode);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_nco_master_slave_gpio_set(adi_ad9081_device_t *device,
						 uint8_t gpio_index,
						 uint8_t output)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(gpio_index > 5);

	if ((gpio_index & 1) == 0) {
		err = adi_ad9081_hal_bf_set(
			device, REG_GPIO_CFG0_ADDR + (gpio_index >> 1), 0x0400,
			(output > 0) ? 10 : 11);
		AD9081_ERROR_RETURN(err);
	} else {
		err = adi_ad9081_hal_bf_set(
			device, REG_GPIO_CFG0_ADDR + (gpio_index >> 1), 0x0404,
			(output > 0) ? 10 : 11);
		AD9081_ERROR_RETURN(err);
	}

	return API_CMS_ERROR_OK;
}

int32_t
adi_ad9081_dac_nco_master_slave_trigger_source_set(adi_ad9081_device_t *device,
						   uint8_t source)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	/* 0: sysref, 1: lmfc rising edge, 2: lmfc falling edge */
	err = adi_ad9081_hal_bf_set(device, REG_NCOSYNC_MS_MODE_ADDR,
				    BF_NCO_SYNC_MS_TRIG_SOURCE_INFO, source);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_nco_master_slave_trigger_set(adi_ad9081_device_t *device)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();

	err = adi_ad9081_hal_bf_set(device, REG_NCO_SYNC_MS_TRIG_ADDR,
				    BF_NCO_SYNC_MS_TRIG_INFO, 1);
	AD9081_ERROR_RETURN(err);

	return API_CMS_ERROR_OK;
}

int32_t adi_ad9081_dac_nco_sync_set(adi_ad9081_device_t *device,
				    uint8_t align_source)
{
	int32_t err;
	AD9081_NULL_POINTER_RETURN(device);
	AD9081_LOG_FUNC();
	AD9081_INVALID_PARAM_RETURN(align_source > 4);

	if (align_source == 0) { /* oneshot */
		err = adi_ad9081_jesd_oneshot_sync(device);
		AD9081_ERROR_RETURN(err);
	} else if (align_source == 1) { /* spi */
		err = adi_ad9081_dac_nco_reset_set(device);
		AD9081_ERROR_RETURN(err);
	} else if (align_source == 2) { /* sysref */
		err = adi_ad9081_dac_nco_sync_sysref_mode_set(device, 0);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_dac_nco_sync_reset_via_sysref_set(device, 1);
		AD9081_ERROR_RETURN(err);
	} else if (align_source == 3) { /* lmfc rising edge */
		err = adi_ad9081_dac_nco_sync_sysref_mode_set(device, 1);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_dac_nco_sync_reset_via_sysref_set(device, 1);
		AD9081_ERROR_RETURN(err);
	} else if (align_source == 4) { /* lmfc falling edge */
		err = adi_ad9081_dac_nco_sync_sysref_mode_set(device, 2);
		AD9081_ERROR_RETURN(err);
		err = adi_ad9081_dac_nco_sync_reset_via_sysref_set(device, 1);
		AD9081_ERROR_RETURN(err);
	}

	return API_CMS_ERROR_OK;
}

/*! @} */
