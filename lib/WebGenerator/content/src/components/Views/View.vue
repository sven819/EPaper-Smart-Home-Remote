<template>
    <v-container>
        <v-card class="mx-auto" hover>
            <v-card-title>

                <v-text-field class="my-4 mx-4" label="Name" v-model="modelValue.name">
                    <template v-slot:append>
                        <v-icon @click="removeItem()" icon="$delete"></v-icon>
                    </template>
                </v-text-field>


            </v-card-title>
            <v-card-item>
                <v-list lines="one">
                    <v-list-item v-for="(n, i)  in modelValue.devices" :key="n" :title="devices.find(x => x.id == n)?.name"
                        :subtitle="n">
                        <template v-slot:append>
                            <v-icon @click="modelValue.devices.splice(i, 1)" icon="$delete"></v-icon>
                        </template>
                    </v-list-item>
                    <v-list-item>
                        <v-select label="Gerät hinzufügen" :items="devices" item-title="name" v-model="tmpDev"
                            item-value="id" @update:modelValue="addDev"></v-select>
                    </v-list-item>
                </v-list>
            </v-card-item>
        </v-card>
    </v-container>
</template>

<script setup lang="ts">
import { Device } from '@/entities/entities';
import { PropType } from 'vue';
import { ref } from 'vue';
import { defineProps, defineEmits } from 'vue';

let tmpDev = ref<string>("");

const props = defineProps({
    modelValue: {
        type: Object,
        required: true
    },
    devices: {
        type: Array as PropType<Device[]>,
        required: true
    }
});
const emit = defineEmits(['update:modelValue', 'remove']);

const removeItem = () => {
    emit('remove', props.modelValue);
}

const addDev = (selectedItem: string) => {
    tmpDev.value = "";
    props.modelValue.devices.push(selectedItem);

}

</script>