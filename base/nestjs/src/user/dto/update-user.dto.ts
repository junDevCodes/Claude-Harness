import { ApiPropertyOptional } from '@nestjs/swagger';
import { IsOptional, IsString, MinLength } from 'class-validator';

export class UpdateUserDto {
  @ApiPropertyOptional({ example: 'newusername', minLength: 2 })
  @IsOptional()
  @IsString()
  @MinLength(2)
  username?: string;

  hashedRefreshToken?: string | null;
}
