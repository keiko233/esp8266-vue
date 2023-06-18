<template>
  <v-row>
    <v-col cols="12">
      <v-card>
        <template v-slot:text>
          <span>{{ $t('home.current_environment') }}: </span>{{ env }}
        </template>
      </v-card>
    </v-col>

    <v-col cols="12" sm="4" v-for="card in cards" :key="card.title">
      <v-card :title="$t(card.title)">
        <template v-slot:text>
          <v-overlay v-model="loading" contained class="align-center justify-center">
            <Loading v-if="loading" />
          </v-overlay>
          <div>
            <p v-for="item in card.items" :key="item.label">
              <span>{{ $t(item.label) }}: </span>{{ getStatusValue(item.key) }}
            </p>
          </div>
        </template>
      </v-card>
    </v-col>
  </v-row>
</template>

<script setup lang="ts">
const env = ref(import.meta.env.VITE_ENV);

const systemInfo = ref();

const loading = ref(false);

const cards = [
  {
    title: 'home.board_info',
    items: [
      { label: 'home.board_name', key: 'system.model' },
      { label: 'home.manufacturer', key: 'system.arch.manufacturer' },
      { label: 'home.board_number', key: 'system.arch.model' }
    ]
  },
  {
    title: 'home.system_info',
    items: [
      { label: 'home.firmware_version', key: 'system.firmware' },
      { label: 'home.esp_sdk_version', key: 'system.esp_sdk' }
    ]
  },
  {
    title: 'home.resource_details',
    items: [
      { label: 'home.cpu_frequency', key: 'system.cpu_freq' },
      { label: 'home.available_memory', key: 'memory.free' }
    ]
  },
  {
    title: 'home.access_point_info',
    items: [
      { label: 'home.ssid', key: 'access_point.ssid' },
      { label: 'home.connected_num', key: 'access_point.num' }
    ]
  },
  {
    title: 'home.static_access_point_info',
    items: [
      { label: 'home.static_ssid', key: 'static_access_point.ssid' },
      { label: 'home.status', key: 'static_access_point.status' }
    ]
  }
];

const getStatus = () => {
  loading.value = true;
  fetch('/api/status')
    .then(response => response.json())
    .then(response => {
      loading.value = false;
      systemInfo.value = response;
      console.log(systemInfo.value);
    })
    .catch(err => console.error(err));
};

const getStatusValue = (key: string) => {
  const keys = key.split('.');
  let value = systemInfo.value;
  for (const k of keys) {
    if (value && value.hasOwnProperty(k)) {
      value = value[k];
    } else {
      return '';
    }
  }
  return value;
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
