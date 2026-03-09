import { z } from 'zod';

export const UpdateUserDto = z.object({
  username: z
    .string()
    .min(3, 'Username must be at least 3 characters')
    .max(30, 'Username must be at most 30 characters')
    .regex(/^[a-zA-Z0-9_]+$/, 'Username can only contain letters, numbers, and underscores')
    .optional(),
  isActive: z.boolean().optional(),
});

export const PaginationQueryDto = z.object({
  page: z.coerce.number().min(1).default(1),
  limit: z.coerce.number().min(1).max(100).default(20),
});

export type UpdateUserInput = z.infer<typeof UpdateUserDto>;
export type PaginationQuery = z.infer<typeof PaginationQueryDto>;
