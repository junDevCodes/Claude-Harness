import { View, Text, StyleSheet } from 'react-native';
import { Link, Stack } from 'expo-router';
import { COLORS } from '@/constants';

export default function NotFoundScreen() {
  return (
    <>
      <Stack.Screen options={{ title: 'Not Found' }} />
      <View style={styles.container}>
        <Text style={styles.title}>404</Text>
        <Text style={styles.subtitle}>This screen doesn't exist.</Text>
        <Link href="/(tabs)" style={styles.link}>
          Go to home screen
        </Link>
      </View>
    </>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    padding: 24,
    backgroundColor: COLORS.background,
  },
  title: {
    fontSize: 64,
    fontWeight: '700',
    color: COLORS.primary,
  },
  subtitle: {
    fontSize: 18,
    color: COLORS.textSecondary,
    marginTop: 8,
    marginBottom: 24,
  },
  link: {
    fontSize: 16,
    color: COLORS.primary,
    fontWeight: '600',
  },
});
