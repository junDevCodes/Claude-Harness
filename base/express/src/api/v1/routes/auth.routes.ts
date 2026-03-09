import { Router, Request, Response, NextFunction } from 'express';
import { AuthService } from '../../../domain/user/auth.service';
import { UserRepository } from '../../../domain/user/user.repository';
import { SignUpDto, LoginDto, RefreshTokenDto } from '../../../dtos/auth.dto';
import { validateBody } from '../../../middleware/validate';
import { authenticate } from '../../../middleware/auth.middleware';

const router = Router();
const userRepo = new UserRepository();
const authService = new AuthService(userRepo);

/** POST /api/v1/auth/sign-up */
router.post(
  '/sign-up',
  validateBody(SignUpDto),
  async (req: Request, res: Response, next: NextFunction) => {
    try {
      const result = await authService.signUp(req.body);
      res.status(201).json(result);
    } catch (err) {
      next(err);
    }
  },
);

/** POST /api/v1/auth/login */
router.post(
  '/login',
  validateBody(LoginDto),
  async (req: Request, res: Response, next: NextFunction) => {
    try {
      const result = await authService.login(req.body);
      res.status(200).json(result);
    } catch (err) {
      next(err);
    }
  },
);

/** POST /api/v1/auth/refresh */
router.post(
  '/refresh',
  validateBody(RefreshTokenDto),
  async (req: Request, res: Response, next: NextFunction) => {
    try {
      const tokens = await authService.refresh(req.body.refreshToken);
      res.status(200).json(tokens);
    } catch (err) {
      next(err);
    }
  },
);

/** GET /api/v1/auth/me */
router.get(
  '/me',
  authenticate,
  async (req: Request, res: Response, next: NextFunction) => {
    try {
      const userRepo2 = new UserRepository();
      const user = await userRepo2.findById(req.user!.sub);
      res.status(200).json({ user });
    } catch (err) {
      next(err);
    }
  },
);

/** DELETE /api/v1/auth/logout */
router.delete(
  '/logout',
  authenticate,
  async (req: Request, res: Response, next: NextFunction) => {
    try {
      await authService.logout(req.user!.sub);
      res.status(204).send();
    } catch (err) {
      next(err);
    }
  },
);

export default router;
