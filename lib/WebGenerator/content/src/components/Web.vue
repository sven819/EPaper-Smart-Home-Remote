<template>
    <v-card class=" ma-2 " hover>
        <v-card-title>
            WebLogin
        </v-card-title>
        <v-card-item>
            <v-text-field label="Username" v-model="modelValue.username"></v-text-field>
            <v-text-field label=" Passwort" v-model="modelValue.password" type="password"></v-text-field>
        </v-card-item>
    </v-card>
</template>


<script setup lang="ts">
import { defineProps, defineEmits, reactive } from 'vue';
import { WebLogin } from '../entities/entities';
import { PropType } from 'vue';

const props = defineProps({
    modelValue: {
        type: Object as PropType<WebLogin>,
        required: true
    }
});

</script>