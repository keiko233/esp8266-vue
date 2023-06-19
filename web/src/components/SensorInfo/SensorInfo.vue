<template>
  <v-card :title="$t('home.sensor_info')">
    <template v-slot:text>
      <v-overlay v-model="loading" contained class="align-center justify-center">
        <Loading v-if="loading" />
      </v-overlay>
      <div>
        <p><span>{{ $t('home.humidity') }}: </span>{{ sensorInfo.humidity }}%</p>
        <p><span>{{ $t('home.temperature') }}: </span>{{ sensorInfo.temperature }}℃</p>
        <p><span>{{ $t('home.photoresistance') }}: </span>{{ sensorInfo.photoresistance }}Ω</p>
      </div>
    </template>
  </v-card>
</template>

<script setup lang="ts">
const loading = ref(false);

const sensorInfo = ref({
  humidity: null,
  temperature: null,
  photoresistance: null
});

const getSensorInfo = () => {
  loading.value = true;
  fetch('/api/sensor')
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      console.log(response);
      sensorInfo.value = response;
    })
    .catch(err => console.error(err));
};

onMounted(() => {
  getSensorInfo();
});
</script>

<style scoped lang="less">
span {
  font-weight: 700;
}
</style>
