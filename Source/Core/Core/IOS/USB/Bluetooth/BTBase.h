// Copyright 2016 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Common/CommonTypes.h"
#include "Core/IOS/Device.h"
#include "Core/IOS/IOS.h"

class PointerWrap;
class SysConf;

namespace IOS::HLE
{
template <typename T>
static void DoStateForMessage(EmulationKernel& ios, PointerWrap& p, std::unique_ptr<T>& message)
{
  u32 request_address = (message != nullptr) ? message->ios_request.address : 0;
  p.Do(request_address);
  if (request_address != 0)
  {
    IOCtlVRequest request{ios.GetSystem(), request_address};
    message = std::make_unique<T>(ios, request);
  }
}

void BackUpBTInfoSection(const SysConf* sysconf);
void RestoreBTInfoSection(SysConf* sysconf);

class BluetoothBaseDevice : public EmulationDevice
{
public:
  using EmulationDevice::EmulationDevice;
  virtual void UpdateSyncButtonState(bool is_held) {}
  virtual void TriggerSyncButtonPressedEvent() {}
  virtual void TriggerSyncButtonHeldEvent() {}

protected:
  static constexpr int ACL_PKT_SIZE = 339;
  static constexpr int ACL_PKT_NUM = 10;
  static constexpr int SCO_PKT_SIZE = 64;
  static constexpr int SCO_PKT_NUM = 0;

  enum USBEndpoint
  {
    HCI_CTRL = 0x00,
    HCI_EVENT = 0x81,
    ACL_DATA_IN = 0x82,
    ACL_DATA_OUT = 0x02
  };
};
}  // namespace IOS::HLE
