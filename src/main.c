/*
 * Copyright 2020 d4rkmen <darkmen@i.ua>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos.h"
#include "mgos_config.h"
#ifdef MGOS_HAVE_WIFI
#include "mgos_wifi.h"
#endif
#include "frozen.h"
#include "mgos_mel_ac.h"

static void net_cb(int ev, void *evd, void *arg) {
  switch (ev) {
    case MGOS_NET_EV_DISCONNECTED:
      LOG(LL_INFO, ("%s", "Net disconnected"));
      break;
    case MGOS_NET_EV_CONNECTING:
      LOG(LL_INFO, ("%s", "Net connecting..."));
      break;
    case MGOS_NET_EV_CONNECTED:
      LOG(LL_INFO, ("%s", "Net connected"));
      break;
    case MGOS_NET_EV_IP_ACQUIRED:
      LOG(LL_INFO, ("%s", "Net got IP address"));
      break;
  }

  (void) evd;
  (void) arg;
}

#ifdef MGOS_HAVE_WIFI
static void wifi_cb(int ev, void *evd, void *arg) {
  switch (ev) {
    case MGOS_WIFI_EV_STA_DISCONNECTED: {
      struct mgos_wifi_sta_disconnected_arg *da =
          (struct mgos_wifi_sta_disconnected_arg *) evd;
      LOG(LL_INFO, ("WiFi STA disconnected, reason %d", da->reason));
      break;
    }
    case MGOS_WIFI_EV_STA_CONNECTING:
      LOG(LL_INFO, ("WiFi STA connecting %p", arg));
      break;
    case MGOS_WIFI_EV_STA_CONNECTED:
      LOG(LL_INFO, ("WiFi STA connected %p", arg));
      break;
    case MGOS_WIFI_EV_STA_IP_ACQUIRED:
      LOG(LL_INFO,
          ("WiFi STA IP acquired: %s", mgos_sys_config_get_wifi_ap_ip()));
      break;
    case MGOS_WIFI_EV_AP_STA_CONNECTED: {
      struct mgos_wifi_ap_sta_connected_arg *aa =
          (struct mgos_wifi_ap_sta_connected_arg *) evd;
      LOG(LL_INFO, ("WiFi AP STA connected MAC %02x:%02x:%02x:%02x:%02x:%02x",
                    aa->mac[0], aa->mac[1], aa->mac[2], aa->mac[3], aa->mac[4],
                    aa->mac[5]));
      break;
    }
    case MGOS_WIFI_EV_AP_STA_DISCONNECTED: {
      struct mgos_wifi_ap_sta_disconnected_arg *aa =
          (struct mgos_wifi_ap_sta_disconnected_arg *) evd;
      LOG(LL_INFO,
          ("WiFi AP STA disconnected MAC %02x:%02x:%02x:%02x:%02x:%02x",
           aa->mac[0], aa->mac[1], aa->mac[2], aa->mac[3], aa->mac[4],
           aa->mac[5]));
      break;
    }
  }
  (void) arg;
}
#endif /* MGOS_HAVE_WIFI */

// static void service_button_handler(int pin, void *arg) {
//   struct mgos_mel_ac *mel = (struct mgos_mel_ac *) arg;
//   if (!mel) return;
// }

static void mel_cb(int ev, void *ev_data, void *arg) {
  switch (ev) {
    case MGOS_MEL_AC_EV_INITIALIZED:
      LOG(LL_INFO, ("MEL init done"));
      break;
    case MGOS_MEL_AC_EV_CONNECTED:
      LOG(LL_INFO, ("connected: %s", *(bool *) ev_data ? "true" : "false"));
      break;
    case MGOS_MEL_AC_EV_CONNECT_ERROR:
      LOG(LL_INFO, ("connect_error: %d", *(uint8_t *) ev_data));
      break;
    case MGOS_MEL_AC_EV_PACKET_WRITE:
      LOG(LL_INFO, ("tx: %s", (char *) ev_data));
      break;
    case MGOS_MEL_AC_EV_PACKET_READ:
      LOG(LL_INFO, ("rx: %s", (char *) ev_data));
      break;
    case MGOS_MEL_AC_EV_PARAMS_CHANGED: {
      struct mgos_mel_ac_params *params = (struct mgos_mel_ac_params *) ev_data;
      LOG(LL_INFO, ("power: %d, mode: %d, setpoint: %.1f, fan: %d, vane_vert: "
                    "%d, vane_horiz: %d, isee: %d",
                    params->power, params->mode, params->setpoint, params->fan,
                    params->vane_vert, params->vane_horiz, params->isee));
      break;
    }
    case MGOS_MEL_AC_EV_ROOMTEMP_CHANGED:
      LOG(LL_INFO, ("room_temp: %.1f", *(float *) ev_data));
      break;
    case MGOS_MEL_AC_EV_PARAMS_SET:
      LOG(LL_INFO, ("new params applied by HVAC"));
      break;
    case MGOS_MEL_AC_EV_PACKET_READ_ERROR:
      LOG(LL_ERROR, ("error: packet crc"));
      break;
    case MGOS_MEL_AC_EV_TIMER:
      // mgos_wdt_feed();
      // LOG(LL_INFO, ("MEL timer"));
      break;
    default:
      LOG(LL_WARN, ("event: %d", ev));
  }
  (void) ev_data;
}

enum mgos_app_init_result mgos_app_init(void) {
  // Register service button, just in case
  //   mgos_gpio_set_mode(mgos_sys_config_get_app_button_gpio(),
  //                      MGOS_GPIO_MODE_INPUT);
  //   mgos_gpio_set_button_handler(mgos_sys_config_get_app_button_gpio(),
  //                                MGOS_GPIO_PULL_UP, MGOS_GPIO_INT_EDGE_POS,
  //                                50, service_button_handler, mel);
  /* MEL-AC events */
  mgos_event_add_group_handler(MGOS_EVENT_GRP_MEL_AC, mel_cb, NULL);

  /* Network connectivity events */
  mgos_event_add_group_handler(MGOS_EVENT_GRP_NET, net_cb, NULL);

#ifdef MGOS_HAVE_WIFI
  mgos_event_add_group_handler(MGOS_EVENT_GRP_WIFI, wifi_cb, NULL);
#endif
  return MGOS_APP_INIT_SUCCESS;
}
