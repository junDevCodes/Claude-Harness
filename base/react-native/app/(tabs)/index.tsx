import { View, Text, StyleSheet, ScrollView } from 'react-native';
import { useAuthStore } from '@/store/authStore';
import { COLORS } from '@/constants';

export default function HomeScreen() {
  const user = useAuthStore((s) => s.user);

  return (
    <ScrollView style={styles.container} contentContainerStyle={styles.content}>
      <Text style={styles.greeting}>
        안녕하세요, {user?.username ?? 'User'} 님 👋
      </Text>
      <Text style={styles.subtitle}>앱에 오신 것을 환영합니다</Text>

      <View style={styles.card}>
        <Text style={styles.cardTitle}>시작하기</Text>
        <Text style={styles.cardText}>
          이 화면을 커스터마이징하려면{' '}
          <Text style={styles.code}>app/(tabs)/index.tsx</Text> 를 편집하세요.
        </Text>
      </View>

      <View style={styles.card}>
        <Text style={styles.cardTitle}>인증 흐름</Text>
        <Text style={styles.cardText}>
          JWT 토큰은 SecureStore에 저장됩니다.{'\n'}
          앱 재시작 시 자동으로 세션이 복원됩니다.
        </Text>
      </View>

      <View style={styles.card}>
        <Text style={styles.cardTitle}>API 클라이언트</Text>
        <Text style={styles.cardText}>
          <Text style={styles.code}>lib/api.ts</Text> 에서 Axios 인터셉터를
          통해 401 응답 시 자동으로 토큰을 갱신합니다.
        </Text>
      </View>
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: { flex: 1, backgroundColor: COLORS.background },
  content: { padding: 24, gap: 16, paddingBottom: 40 },
  greeting: {
    fontSize: 26,
    fontWeight: '700',
    color: COLORS.text,
  },
  subtitle: {
    fontSize: 16,
    color: COLORS.textSecondary,
    marginBottom: 8,
  },
  card: {
    backgroundColor: COLORS.surface,
    borderRadius: 12,
    padding: 20,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.06,
    shadowRadius: 8,
    elevation: 2,
  },
  cardTitle: {
    fontSize: 17,
    fontWeight: '600',
    marginBottom: 8,
    color: COLORS.text,
  },
  cardText: {
    fontSize: 14,
    color: COLORS.textSecondary,
    lineHeight: 22,
  },
  code: {
    fontFamily: 'monospace',
    fontSize: 13,
    color: COLORS.primary,
  },
});
