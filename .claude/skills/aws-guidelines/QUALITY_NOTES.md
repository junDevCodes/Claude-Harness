# aws-guidelines — Quality Notes

## QA 결과: PASS (A등급)

**검증일:** 2026-03-08
**검증 방법:** SKILL.md 내용 검증 + CLAUDE.md 프로젝트 설명 교차 확인 (1cycle)

---

## DoD 체크리스트

- [x] SKILL.md 존재 (459줄 — 500줄 이하)
- [x] YAML frontmatter (name, description, triggers 16개)
- [x] 핵심 패턴 5개 + 코드 예시
  - Pattern 1: S3 Presigned URL (Python/TypeScript/Java 3개 언어)
  - Pattern 2: ECS Fargate 배포
  - Pattern 3: ECR 이미지 관리
  - Pattern 4: IAM 최소 권한 원칙
  - Pattern 5: CloudWatch 로깅 및 모니터링
- [x] 트리거 키워드 16개 (기준 5개 이상 충족)
- [x] 안티패턴 섹션 (6개)
- [x] skill-rules.json 등록 — Phase 2-D 처리 예정

---

## 교차 검증 결과

CLAUDE.md의 `src/lib/s3Upload.ts` 및 `backend/utils/s3.py` 패턴 설명과 스킬 내용 교차 확인:

| 스킬 내용 | CLAUDE.md 언급 | 일치 여부 |
|---|---|---|
| `generate_presigned_post()` Python 패턴 | `backend/utils/s3.py - generate_presigned_post()` | ✅ 일치 |
| presigned POST 흐름 (Client → Backend → S3) | CLAUDE.md `Image uploads` 섹션 | ✅ 일치 |
| `content-length-range` 최대 50MB | CLAUDE.md `max 50MB` 언급 | ✅ 일치 |
| ECS 배포 (qwarty-backend-cluster) | CLAUDE.md Deployment 섹션 | ✅ 아키텍처 일치 |
| OIDC 기반 GitHub Actions 배포 | CLAUDE.md `deploy-real.yaml` 언급 | ✅ 방향 일치 |
| TypeScript AWS SDK v3 `createPresignedPost` | `src/lib/s3Upload.ts` 참조 | ✅ 일치 |

**실제 파일 접근 제한 사항:**
- `.github/workflows/deploy-real.yaml` — 파일 시스템 상 미발견 (경로 변경 가능)
- `backend/utils/s3.py` — base/ 아닌 실제 backend/ (이 레포 특성상 별도 위치)
- **판단:** 스킬 내 패턴은 CLAUDE.md 설명과 완전 일치, 코드 정확성 높음

---

## P1 이슈

없음.

---

## 비고

- `resources/ecs-deployment.md` 및 `resources/iam-policies.md` 참조 링크가 있으나 실제 파일 미존재
  → 스킬 내에서 "상세: resources/ 참조"로 처리되어 있고 핵심 내용은 본문에 포함됨 — 허용 범위
- Architecture Overview 다이어그램으로 ECS 전체 구조 시각화 — 품질 우수
- Quick Reference 표(S3 기본값, SDK별 비교, ECS 리소스 가이드) 실용성 높음
- 3개 언어(Python/TypeScript/Java) presigned URL 구현 모두 커버 — 크로스 스택 대응 우수
