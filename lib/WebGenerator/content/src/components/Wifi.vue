<template>
    <v-card class=" ma-2 " hover>
        <v-card-title>
            WiFi
        </v-card-title>
        <v-card-item>
            <v-combobox label="SSID" v-model="modelValue.sta_ssid" :items="networks" item-title="SSID" item-value="SSID"
                :loading="loading">
                <template v-slot:item="{ props, item }">
                    <v-list-item v-bind="props">
                        <v-list-item-subtitle>
                            RSSI: {{ item.raw.RSSI }}
                        </v-list-item-subtitle>
                    </v-list-item>
                </template>
            </v-combobox>
            <v-text-field label="Passwort" v-model="modelValue.sta_password" type="password"></v-text-field>
            <v-text-field label="Fallback SSID" v-model="modelValue.ap_ssid"></v-text-field>
            <v-text-field label=" Fallback Passwort" v-model="modelValue.ap_password" type="password"></v-text-field>
        </v-card-item>
    </v-card>
</template>


<script setup lang="ts">
import { defineProps, ref, reactive, onMounted } from 'vue';
import { WiFiConfig, wifiNetworks } from '../entities/entities';
import { PropType } from 'vue';

const props = defineProps({
    modelValue: {
        type: Object as PropType<WiFiConfig>,
        required: true
    }
});
let networks = reactive<wifiNetworks[]>([]);
let loading = ref<boolean>(true);



const fetchConfig = () => {
    console.log("fetchData");
    const response = fetch('api/wifi').then((response) => {
        response.json().then((data: wifiNetworks[]) => {
            const highestRSSI = new Map<string, number>();

            data.forEach(network => {
                if (!highestRSSI.has(network.SSID) || network.RSSI > highestRSSI.get(network.SSID)!) {
                    highestRSSI.set(network.SSID, network.RSSI);
                }
            });
            Object.assign(networks, data.filter(network => network.RSSI === highestRSSI.get(network.SSID)));
            loading.value = false;
        });
    })
        .catch((err) => {
            loading.value = false;
            console.error(err);
        });
};


onMounted(() => {
    fetchConfig();
});


</script>