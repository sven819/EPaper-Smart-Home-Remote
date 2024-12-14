<template>
    <v-card class="ma-2" hover>
        <v-card-title>
            View<v-icon @click="addItem()" icon="$add"></v-icon>

        </v-card-title>
        <v-card-item>
            <v-list lines="one">
                <v-list-item v-for="(view, i) in modelValue" :key="i">
                    <view-comp v-model="modelValue[i]" :devices="devices" @remove="modelValue.splice(i, 1)"></view-comp>
                </v-list-item>
            </v-list>
        </v-card-item>
    </v-card>
</template>

<script setup lang="ts">
import { defineProps } from 'vue';
import { PropType } from 'vue';

import { View, Device } from '../../entities/entities';
import ViewComp from '@/components/Views/View.vue'

const props = defineProps({
    modelValue: {
        type: Array as PropType<View[]>,
        required: true
    },
    devices: {
        type: Array as PropType<Device[]>,
        required: true
    }
});

const addItem = () => {
    props.modelValue.push({ name: "", devices: [] });
}

</script>