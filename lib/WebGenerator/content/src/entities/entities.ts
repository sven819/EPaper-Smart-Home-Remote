export interface Configuration {
  provider: string;
  devices: Device[];
  wifi: WiFiConfig;
  views: View[];
  providerParams: ProviderParams;
  webLogin: WebLogin;
}

export interface Device {
  name: string;
  type: string;
  id: string;
}

export interface WebLogin {
  username: string;
  password: string;
}

export interface WiFiConfig {
  ap_ssid: string;
  ap_password: string;
  sta_ssid: string;
  sta_password: string;
}

export interface View {
  name: string;
  devices: string[];
}

export interface ProviderParams {
  host: string;
  port: number;
  ssl: boolean;
  apiToken: string;
}

export interface wifiNetworks {
  SSID: string;
  RSSI: number;
}
