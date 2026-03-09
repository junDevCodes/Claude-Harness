import type { FullConfig } from "@playwright/test";

async function globalSetup(_config: FullConfig): Promise<void> {
  return;
}

export default globalSetup;
