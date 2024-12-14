<template>
    <v-card class=" ma-2 " hover>
        <v-card-title>
            Provider
        </v-card-title>
        <v-card-item>
            <v-select label="Provider" :items="['HA', 'Dummy']" v-model="modelValue.provider"></v-select>
            <v-text-field label="Host" v-model="modelValue.providerParams.host"></v-text-field>
            <v-text-field label="Port" v-model="modelValue.providerParams.port"></v-text-field>
            <v-text-field label="API Token" v-model="modelValue.providerParams.apiToken" type="password"></v-text-field>
            <v-checkbox label="SSL" v-model="modelValue.providerParams.ssl"></v-checkbox>
        </v-card-item>
    </v-card>
</template>


<script setup lang="ts">
import { ref } from 'vue';
import { defineProps, defineEmits, reactive } from 'vue';

const props = defineProps({
    modelValue: {
        type: Object,
        required: true
    }
});


</script>