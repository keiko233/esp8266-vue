<template>
  <v-row>
    <v-col cols="12">
      <v-card>
        <template v-slot:text>
          <span>{{ $t('home.current_environment') }}: </span>{{ env }}
        </template>
      </v-card>
    </v-col>

    <v-col cols="12" sm="4">
      <v-card :title="$t('home.board_info')">
        <template v-slot:text>
          <Loading v-if="!status" />
          <div v-else>
            <p><span>{{ $t('home.board_name') }}: </span>{{ status.system.model }}</p>
            <p><span>{{ $t('home.manufacturer') }}: </span>{{ status.system.arch.manufacturer }}</p>
            <p><span>{{ $t('home.board_number') }}: </span>{{ status.system.arch.model }}</p>
          </div>
        </template>
      </v-card>
    </v-col>

    <v-col cols="12" sm="4">
      <v-card :title="$t('home.system_info')">
        <template v-slot:text>
          <Loading v-if="!status" />
          <div v-else>
            <p><span>{{ $t('home.firmware_version') }}: </span>{{ status.system.firmware }}</p>
            <p><span>{{ $t('home.esp_sdk_version') }}: </span>{{ status.system.esp_sdk }}</p>
          </div>
        </template>
      </v-card>
    </v-col>

    <v-col cols="12" sm="4">
      <v-card :title="$t('home.resource_details')">
        <template v-slot:text>
          <Loading v-if="!status" />
          <div v-else>
            <p><span>{{ $t('home.cpu_frequency') }}: </span>{{ status.system.cpu_freq }} Mhz</p>
            <p><span>{{ $t('home.available_memory') }}: </span>{{ status.memory.free }}</p>
          </div>
        </template>
      </v-card>
    </v-col>

    <v-col cols="12" sm="4">
      <v-card :title="$t('home.access_point_info')">
        <template v-slot:text>
          <Loading v-if="!status" />
          <div v-else>
            <p><span>{{ $t('home.ssid') }}: </span>{{ status.access_point.ssid }}</p>
            <p><span>{{ $t('home.connected_num') }}: </span>{{ status.access_point.num }}</p>
          </div>
        </template>
      </v-card>
    </v-col>

    <v-col cols="12" sm="4">
      <v-card :title="$t('home.static_access_point_info')">
        <template v-slot:text>
          <Loading v-if="!status" />
          <div v-else>
            <p><span>{{ $t('home.static_ssid') }}: </span>{{ status.static_access_point.ssid }}</p>
            <p><span>{{ $t('home.status') }}: </span>{{ status.static_access_point.status }}</p>
          </div>
        </template>
      </v-card>
    </v-col>
  </v-row>
</template>

<script setup lang="ts">
const env = ref(import.meta.env.VITE_ENV);

const status = ref();

const getStatus = () => {
  console.log(import.meta.env.VITE_API_URL + '/status');
  fetch('/api/status')
    .then(response => response.json())
    .then(response => {
      status.value = response;
      console.log(status.value);
    })
    .catch(err => console.error(err));
};

onMounted(() => {
  getStatus();
});
</script>

<style scoped lang="less">
span {
  font-weight: 700;
}
</style>
