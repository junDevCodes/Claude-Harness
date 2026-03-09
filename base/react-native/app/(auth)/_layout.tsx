import { Redirect, Stack } from 'expo-router';
import { useAuthStore } from '@/store/authStore';

/**
 * Auth 그룹 레이아웃.
 * 이미 인증된 사용자는 (tabs)로 redirect.
 */
export default function AuthLayout() {
  const isAuthenticated = useAuthStore((s) => s.isAuthenticated);

  if (isAuthenticated) {
    return <Redirect href="/(tabs)" />;
  }

  return (
    <Stack
      screenOptions={{
        headerShown: false,
        animation: 'slide_from_right',
      }}
    />
  );
}
