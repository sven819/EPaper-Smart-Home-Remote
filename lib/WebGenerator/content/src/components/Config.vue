<template>
  <v-container>
    <wifi v-model="config.wifi"> </wifi>
    <web v-model="config.webLogin"></web>
    <provider v-model="config"></provider>
    <device-list :devices="config.devices" />
    <view-list v-model="config.views" :devices="config.devices" />
  </v-container>

  <div class="pa-4 text-end">
    <v-btn class="text-none" color="medium-emphasis" min-width="92" @click="fetchConfig()" rounded variant="outlined">
      Zurücksetzen
    </v-btn>
    <v-btn class="text-none mx-2" color="medium-emphasis" min-width="92" @click="saveConfig()" rounded variant="outlined">
      Speichern
    </v-btn>
  </div>
</template>

<script setup lang="ts">
import { reactive, ref, onMounted } from 'vue';
import DeviceList from '@/components/Device/DeviceList.vue'
import ViewList from '@/components/Views/ViewList.vue'

import Provider from '@/components/Provider.vue'
import Wifi from '@/components/Wifi.vue'
import Web from '@/components/Web.vue'


import { Configuration } from '../entities/entities';

let config = reactive<Configuration>(
  {
    provider: "",
    devices: [],
    wifi: {
      ap_ssid: "",
      ap_password: "",
      sta_ssid: "",
      sta_password: ""
    },
    views: [],
    providerParams: {
      host: "",
      port: 0,
      ssl: false,
      apiToken: ""
    },
    webLogin: {
      username: "admin",
      password: "admin"
    }
  });


const fetchConfig = () => {
  console.log("fetchData");
  const response = fetch('api/settings').then((response) => {
    response.json().then((data) => {
      Object.assign(config, data);
    });
  })
    .catch((err) => {
      console.error(err);
    });
};

const saveConfig = () => {
  fetch('api/settings', {
    method: 'POST',
    mode: 'cors',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(config) // Daten als JSON-String senden
  })
    .then(response => {
      if (!response.ok) {
        throw new Error('Network response was not ok ' + response.statusText);
      }
      return response.json(); // oder response.text() wenn die Antwort kein JSON ist
    })
    .then(data => {
      console.log('Success:', data); // Erfolgreiche Antwort verarbeiten
    })
    .catch(error => {
      console.error('Error:', error); // Fehler behandeln
    });
};

onMounted(() => {
  fetchConfig();
});

</script>
