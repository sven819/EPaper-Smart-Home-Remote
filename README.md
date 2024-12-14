# EPaper Smart Home Remote

A remote control tailored for Home Assistant, designed for energy-efficient ePaper displays. Built on ESP32, this project allows seamless configuration via a web interface and features automatic periodic updates for the displayed information.

<img width="888" alt="image" src="https://github.com/user-attachments/assets/73b4e8f3-fb87-42a1-8c3c-8f63c050cfd3" />


## Features

- **Home Assistant Integration**: Control your smart home devices effortlessly.  
- **Cyclic Display Updates**: Automatically refresh the display at defined intervals to ensure up-to-date information.  
- **Supported Hardware**:  
  - **M5Paper**  
  - **LilyGo T5**  
- **Web Interface**: Configure smart home integration and device settings directly from your browser.  
- **Debugging Support**: Test and develop with **Sunton S5**.  

## Getting Started

### Prerequisites

- M5Paper or LilyGo T5 hardware  
- Home Assistant setup  
- PlatformIO installed  

### Installation

1. Clone the repository:
2. Open the project in PlatformIO.  
3. Flash the firmware to your ESP32 device.

### Configuration via Web Interface

1. After flashing, connect the remote to your Wi-Fi network.  
2. Access the provided IP address in your browser.  
3. Configure your **Home Assistant integration** and the devices to be displayed.

## Usage

The remote periodically refreshes the display to show the latest device states from Home Assistant. The ePaper screen ensures minimal power usage while maintaining visibility.

## Contribution

Contributions are welcome! Submit pull requests or report issues directly in the repository.
