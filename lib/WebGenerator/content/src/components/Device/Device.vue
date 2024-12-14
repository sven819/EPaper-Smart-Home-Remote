<template>
    <v-container>

        <v-row>
            <v-col cols="12" md="3">
                <v-text-field label="Name" v-model="modelValue.name"></v-text-field>
            </v-col>
            <v-col cols="12" md="6">

                <v-text-field label="ID" v-model="modelValue.id"></v-text-field>
            </v-col>
            <v-col cols="12" md="3">

                <v-select label="Type" :items="['Light', 'Climate', 'Sensor', 'Shutter', 'ColorLight', 'DimmLight', 'Media']"
                    v-model="modelValue.type"></v-select>
            </v-col>
        </v-row>

    </v-container>
</template>

<script setup lang="ts">
import { defineProps, defineEmits, reactive } from 'vue';

const props = defineProps({
    modelValue: {
        type: Object,
        required: true
    }
});
const emit = defineEmits(['update:modelValue', 'remove']);


const removeItem = () => {
    emit('remove', props.modelValue);
}

</script>