#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

enum USB_DESCRIPTORS_ITF {
  ITF_NUM_CDC_0 = 0,
  ITF_NUM_CDC_0_DATA,
  ITF_NUM_AUDIO_CONTROL,
  ITF_NUM_AUDIO_STREAMING,
  ITF_NUM_TOTAL
};

#define EPNUM_CDC_0_NOTIF   0x81
#define EPNUM_CDC_0_OUT     0x02
#define EPNUM_CDC_0_IN      0x82
#define EPNUM_AUDIO_IN      0x83

#endif /* USB_DESCRIPTORS_H_ */