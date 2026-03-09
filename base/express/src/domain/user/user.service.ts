import { UserRepository, UpdateUserInput } from './user.repository';
import { SafeUser } from './user.model';
import { AppError } from '../../errors/app.error';
import { PaginationOptions, PaginatedResult } from '../shared/base.repository';

export class UserService {
  constructor(private readonly userRepo: UserRepository) {}

  async getUserById(id: string): Promise<SafeUser> {
    const user = await this.userRepo.findById(id);
    if (!user) {
      throw new AppError('User not found', 404, 'USER_NOT_FOUND');
    }
    return user;
  }

  async getUsers(options: PaginationOptions): Promise<PaginatedResult<SafeUser>> {
    return this.userRepo.findAll(options);
  }

  async updateUser(id: string, data: UpdateUserInput): Promise<SafeUser> {
    const user = await this.userRepo.findById(id);
    if (!user) {
      throw new AppError('User not found', 404, 'USER_NOT_FOUND');
    }

    if (data.username && data.username !== user.username) {
      const existing = await this.userRepo.findByUsername(data.username);
      if (existing) {
        throw AppError.conflict('Username already exists', 'USERNAME_ALREADY_EXISTS');
      }
    }

    return this.userRepo.update(id, data);
  }

  async deleteUser(id: string): Promise<void> {
    const user = await this.userRepo.findById(id);
    if (!user) {
      throw new AppError('User not found', 404, 'USER_NOT_FOUND');
    }
    await this.userRepo.delete(id);
  }
}
