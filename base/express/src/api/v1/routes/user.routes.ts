import { Router, Request, Response, NextFunction } from 'express';
import { UserService } from '../../../domain/user/user.service';
import { UserRepository } from '../../../domain/user/user.repository';
import { UpdateUserDto, PaginationQueryDto } from '../../../dtos/user.dto';
import { validateBody, validateQuery } from '../../../middleware/validate';
import { authenticate, requireRole } from '../../../middleware/auth.middleware';

const router = Router();
const userRepo = new UserRepository();
const userService = new UserService(userRepo);

// 모든 유저 라우트에 인증 필요
router.use(authenticate);

/** GET /api/v1/users */
router.get(
  '/',
  validateQuery(PaginationQueryDto),
  async (req: Request, res: Response, next: NextFunction) => {
    try {
      const { page, limit } = req.query as unknown as { page: number; limit: number };
      const result = await userService.getUsers({ page, limit });
      res.status(200).json(result);
    } catch (err) {
      next(err);
    }
  },
);

/** GET /api/v1/users/:id */
router.get(
  '/:id',
  async (req: Request, res: Response, next: NextFunction) => {
    try {
      const user = await userService.getUserById(req.params.id);
      res.status(200).json({ user });
    } catch (err) {
      next(err);
    }
  },
);

/** PATCH /api/v1/users/:id */
router.patch(
  '/:id',
  validateBody(UpdateUserDto),
  async (req: Request, res: Response, next: NextFunction) => {
    try {
      // 본인 또는 관리자만 수정 가능
      if (req.user!.sub !== req.params.id && req.user!.role !== 'ADMIN') {
        res.status(403).json({
          error: { code: 'FORBIDDEN', message: 'You can only update your own profile' },
        });
        return;
      }
      const user = await userService.updateUser(req.params.id, req.body);
      res.status(200).json({ user });
    } catch (err) {
      next(err);
    }
  },
);

/** DELETE /api/v1/users/:id — 관리자 전용 */
router.delete(
  '/:id',
  requireRole('ADMIN'),
  async (req: Request, res: Response, next: NextFunction) => {
    try {
      await userService.deleteUser(req.params.id);
      res.status(204).send();
    } catch (err) {
      next(err);
    }
  },
);

export default router;
