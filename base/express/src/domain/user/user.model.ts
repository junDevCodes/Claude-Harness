import { UserRole } from '@prisma/client';

export { UserRole };

export interface User {
  id: string;
  email: string;
  username: string;
  passwordHash: string;
  role: UserRole;
  isActive: boolean;
  createdAt: Date;
  updatedAt: Date;
}

export interface SafeUser {
  id: string;
  email: string;
  username: string;
  role: UserRole;
  isActive: boolean;
  createdAt: Date;
  updatedAt: Date;
}

/** passwordHash 제거한 공개 정보 */
export function toSafeUser(user: User): SafeUser {
  const { passwordHash: _omitted, ...safe } = user;
  return safe;
}
