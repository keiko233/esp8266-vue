<template>
  <v-card :title="$t('settings.shortcuts')">
    <template v-slot:text>
      <div>
        <v-btn @click="softReboot" block class="mb-2">{{ $t('settings.soft_reboot') }}</v-btn>
        <v-btn @click="restart" block>{{ $t('settings.restart') }}</v-btn>
      </div>
    </template>
  </v-card>
</template>

<script setup lang="ts">
const softReboot = () => {
  fetch('/api/softreboot')
};

const restart = () => {
  fetch('/api/restart')
};
</script>
