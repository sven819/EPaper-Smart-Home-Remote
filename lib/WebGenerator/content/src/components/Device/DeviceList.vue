<template>
  <v-card class="ma-2" hover>
    <v-card-title>
      Geräte<v-icon @click="addItem()" icon="$add"></v-icon>
    </v-card-title>
    <v-card-item>

      <v-list lines="one">

        <v-list-item v-for="(dev, i) in devices" :key="i">
          <device-comp v-model="devices[i]" @remove="devices.splice(i, 1)"></device-comp>
          <template v-slot:append>
            <v-icon @click="devices.splice(i, 1)" icon="$delete"></v-icon>
          </template>
        </v-list-item>

      </v-list>
    </v-card-item>
  </v-card>
</template>

<script setup lang="ts">
import { defineProps } from 'vue';
import { PropType } from 'vue';

import { Device } from '../../entities/entities';
import DeviceComp from '@/components/Device/Device.vue'
import { mdiDevices } from '@mdi/js';
const props = defineProps({
  devices: {
    type: Array as PropType<Device[]>,
    required: true
  }
});

const addItem = () => {
  props.devices.push({ name: "", id: "", type: "" });
}


</script>