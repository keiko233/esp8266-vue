<template>
  <v-card :title="$t('settings.serial_settings')">
    <template v-slot:text>
      <v-overlay v-model="loading" contained class="align-center justify-center">
        <Loading v-if="loading" />
      </v-overlay>
      <div>
        <v-text-field v-model="serialInfo.timeout_duration" :label="$t('settings.serial_timeout_duration')" variant="solo" />
        <v-btn @click="updateSerialInfo">{{ $t('submit') }}</v-btn>
      </div>
    </template>
  </v-card>
</template>

<script setup lang="ts">
const loading = ref(false);

const serialInfo = ref({
  timeout_duration: null
});

const getSerialInfo = () => {
  loading.value = true;
  fetch('/api/serial')
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      serialInfo.value = response;
    })
    .catch(err => console.error(err));
};

const updateSerialInfo = () => {
  loading.value = true;
  fetch('/api/serial', {
    method: 'POST',
    headers: { 'content-type': 'application/x-www-form-urlencoded' },
    body: new URLSearchParams({
      timeout_duration: serialInfo.value.timeout_duration,
    })
  })
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      console.log(response);
    })
    .catch(err => console.error(err));
};

onMounted(() => {
  getSerialInfo();
});
</script>