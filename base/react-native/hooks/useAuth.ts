import { useAuthStore } from '@/store/authStore';

/**
 * authStore 셀렉터 래퍼.
 * 컴포넌트에서 useAuthStore 직접 사용 대신 이 훅을 사용하면
 * 재렌더링 범위를 셀렉터로 최소화할 수 있습니다.
 */
export function useAuth() {
  const user = useAuthStore((s) => s.user);
  const isAuthenticated = useAuthStore((s) => s.isAuthenticated);
  const isLoading = useAuthStore((s) => s.isLoading);
  const login = useAuthStore((s) => s.login);
  const logout = useAuthStore((s) => s.logout);
  const register = useAuthStore((s) => s.register);

  return { user, isAuthenticated, isLoading, login, logout, register };
}
