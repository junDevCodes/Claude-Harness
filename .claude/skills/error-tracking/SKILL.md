---
name: error-tracking
description: Add Sentry v8 error tracking and performance monitoring to your project services. Use this skill when adding error handling, creating new controllers, instrumenting cron jobs, or tracking database performance. ALL ERRORS MUST BE CAPTURED TO SENTRY - no exceptions.
triggers:
  - sentry
  - error tracking
  - captureException
  - monitoring
  - performance tracking
  - error handling
  - sentry integration
---

# Sentry v8 Error Tracking & Performance Monitoring

## Purpose
This skill enforces comprehensive Sentry error tracking and performance monitoring across all services following Sentry v8 patterns.

## When to Use This Skill
- Adding error handling to any code
- Creating new controllers or routes
- Instrumenting cron jobs
- Tracking database performance
- Adding performance spans
- Handling workflow errors

## CRITICAL RULE

**ALL ERRORS MUST BE CAPTURED TO SENTRY** - No exceptions. Never use console.error alone.

## Sentry Integration Patterns

### 1. Controller Error Handling

```typescript
// CORRECT - Use BaseController
import { BaseController } from '../controllers/BaseController';

export class MyController extends BaseController {
    async myMethod() {
        try {
            // ... your code
        } catch (error) {
            this.handleError(error, 'myMethod'); // Automatically sends to Sentry
        }
    }
}
```

### 2. Route Error Handling (Without BaseController)

```typescript
import * as Sentry from '@sentry/node';

router.get('/route', async (req, res) => {
    try {
        // ... your code
    } catch (error) {
        Sentry.captureException(error, {
            tags: { route: '/route', method: 'GET' },
            extra: { userId: req.user?.id }
        });
        res.status(500).json({ error: 'Internal server error' });
    }
});
```

### 3. Workflow Error Handling

```typescript
import * as Sentry from '@sentry/node';

Sentry.withScope((scope) => {
    scope.setTag('workflow', 'order-processing');
    scope.setTag('step', 'payment');
    scope.setContext('workflow', {
        instanceId: 123,
        stepId: 456,
        userId: 'user-123',
        operation: 'stepCompletion',
    });
    Sentry.captureException(error);
});
```

### 4. Cron Jobs (MANDATORY Pattern)

```typescript
#!/usr/bin/env node
// FIRST LINE after shebang - CRITICAL!
import '../instrument';
import * as Sentry from '@sentry/node';

async function main() {
    return await Sentry.startSpan({
        name: 'cron.job-name',
        op: 'cron',
        attributes: {
            'cron.job': 'job-name',
            'cron.startTime': new Date().toISOString(),
        }
    }, async () => {
        try {
            // Your cron job logic
        } catch (error) {
            Sentry.captureException(error, {
                tags: {
                    'cron.job': 'job-name',
                    'error.type': 'execution_error'
                }
            });
            console.error('[Job] Error:', error);
            process.exit(1);
        }
    });
}

main()
    .then(() => {
        console.log('[Job] Completed successfully');
        process.exit(0);
    })
    .catch((error) => {
        console.error('[Job] Fatal error:', error);
        process.exit(1);
    });
```

### 5. Database Performance Monitoring

```typescript
import * as Sentry from '@sentry/node';

const result = await Sentry.startSpan({
    name: 'db.findMany',
    op: 'db.query',
    attributes: {
        'db.system': 'postgresql',
        'db.operation': 'findMany',
        'db.table': 'user_profile',
    }
}, async () => {
    return await prisma.userProfile.findMany({ take: 5 });
});
```

### 6. Async Operations with Spans

```typescript
import * as Sentry from '@sentry/node';

const result = await Sentry.startSpan({
    name: 'operation.name',
    op: 'operation.type',
    attributes: {
        'custom.attribute': 'value'
    }
}, async () => {
    // Your async operation
    return await someAsyncOperation();
});
```

## Error Levels

Use appropriate severity levels:

- **fatal**: System is unusable (database down, critical service failure)
- **error**: Operation failed, needs immediate attention
- **warning**: Recoverable issues, degraded performance
- **info**: Informational messages, successful operations
- **debug**: Detailed debugging information (dev only)

## Required Context

```typescript
import * as Sentry from '@sentry/node';

Sentry.withScope((scope) => {
    // ALWAYS include these if available
    scope.setUser({ id: userId });
    scope.setTag('service', 'api');
    scope.setTag('environment', process.env.NODE_ENV);

    // Add operation-specific context
    scope.setContext('operation', {
        type: 'workflow.start',
        workflowCode: 'ORDER_PROCESSING',
        entityId: 123
    });

    Sentry.captureException(error);
});
```

## Integration Examples

### Node.js / Express Initialization

**Location**: `src/instrument.ts`

```typescript
import * as Sentry from '@sentry/node';
import { nodeProfilingIntegration } from '@sentry/profiling-node';

Sentry.init({
    dsn: process.env.SENTRY_DSN,
    environment: process.env.NODE_ENV || 'development',
    integrations: [
        nodeProfilingIntegration(),
    ],
    tracesSampleRate: 0.1,
    profilesSampleRate: 0.1,
});
```

**Express Setup** (Sentry v8):

```typescript
import * as Sentry from '@sentry/node';
import express from 'express';

const app = express();

// v8: Use setupExpressErrorHandler instead of Handlers
// Routes go here...

Sentry.setupExpressErrorHandler(app);
```

### Key Helpers (Recommended Structure)

- `src/instrument.ts` - Sentry initialization (import first!)
- `src/utils/sentryHelper.ts` - Domain-specific error helpers
- `src/utils/databasePerformance.ts` - DB query tracking
- `src/controllers/BaseController.ts` - Controller error handling base

## Configuration

```ini
[sentry]
dsn = your-sentry-dsn
environment = development
tracesSampleRate = 0.1
profilesSampleRate = 0.1

[databaseMonitoring]
enableDbTracing = true
slowQueryThreshold = 100
logDbQueries = false
dbErrorCapture = true
enableN1Detection = true
```

## Testing Sentry Integration

```bash
# Test basic error capture
curl http://localhost:3000/api/sentry/test-error

# Test custom span tracking
curl http://localhost:3000/api/sentry/test-performance

# Test database monitoring
curl http://localhost:3000/api/sentry/test-database-performance
```

After running test endpoints, verify in Sentry dashboard:
1. Error appears in Issues with correct tags
2. Performance transaction recorded with spans
3. User context and tags attached correctly

## Performance Monitoring

### Requirements

1. **All API endpoints** must have transaction tracking
2. **Database queries > 100ms** are automatically flagged
3. **N+1 queries** are detected and reported
4. **Cron jobs** must track execution time

### Transaction Tracking (Sentry v8)

```typescript
import * as Sentry from '@sentry/node';

// v8: Use startSpan instead of deprecated startTransaction
const result = await Sentry.startSpan({
    op: 'operation.type',
    name: 'Operation Name',
}, async (span) => {
    // Your operation
    // Nested spans for sub-operations
    return await Sentry.startSpan({
        op: 'db.query',
        name: 'Fetch user data',
    }, async () => {
        return await fetchUserData();
    });
});
```

## Anti-Patterns

### 1. catch 블록에서 Sentry 미전송
```typescript
// BAD: 에러를 콘솔에만 출력
try {
    await riskyOperation();
} catch (error) {
    console.error(error);  // Sentry에 전송되지 않음!
}

// GOOD: Sentry에 캡처 후 필요시 콘솔 출력
try {
    await riskyOperation();
} catch (error) {
    Sentry.captureException(error);
    throw error;  // 또는 적절한 에러 응답
}
```

### 2. 전역 try-catch로 모든 에러 뭉개기
```typescript
// BAD: 최상위에서 모든 에러를 삼키고 200 반환
app.use(async (req, res, next) => {
    try {
        await next();
    } catch (e) {
        res.status(200).json({ ok: true }); // 에러 은폐!
    }
});

// GOOD: 에러별 분기 + Sentry context 포함
app.use(async (req, res, next) => {
    try {
        await next();
    } catch (error) {
        Sentry.captureException(error, {
            tags: { route: req.path, method: req.method },
        });
        res.status(500).json({ error: 'Internal server error' });
    }
});
```

### 3. PII 데이터 Sentry 전송
```typescript
// BAD: 민감 정보를 Sentry에 그대로 전송
Sentry.setUser({ email: user.email, password: user.password, ssn: user.ssn });

// GOOD: 최소 식별 정보만 전송
Sentry.setUser({ id: user.id });
```

### 4. deprecated startTransaction 사용
```typescript
// BAD: Sentry v7 API (v8에서 deprecated)
const transaction = Sentry.startTransaction({ op: 'task', name: 'My Task' });
try { /* ... */ } finally { transaction.finish(); }

// GOOD: Sentry v8 startSpan API
await Sentry.startSpan({ op: 'task', name: 'My Task' }, async () => {
    // Your operation — span auto-finishes
});
```

### 5. 에러 컨텍스트 없이 캡처
```typescript
// BAD: 에러만 전송, 디버깅 단서 없음
Sentry.captureException(error);

// GOOD: tags + context로 디버깅 단서 제공
Sentry.captureException(error, {
    tags: { service: 'payment', operation: 'charge' },
    extra: { orderId: order.id, amount: order.total },
});
```

### 6. instrument.ts 임포트 순서 위반 (Cron Jobs)
```typescript
// BAD: Sentry 초기화 전에 다른 모듈 로드
import { myService } from '../services/myService';
import '../instrument'; // 너무 늦음!

// GOOD: instrument.ts를 반드시 첫 번째로 임포트
import '../instrument';
import * as Sentry from '@sentry/node';
import { myService } from '../services/myService';
```

## Implementation Checklist

When adding Sentry to new code:

- [ ] Imported Sentry or appropriate helper
- [ ] All try/catch blocks capture to Sentry
- [ ] Added meaningful context to errors (tags, extra)
- [ ] Used appropriate error level
- [ ] No sensitive data (PII) in error messages
- [ ] Added performance tracking for slow operations
- [ ] Tested error handling paths
- [ ] For cron jobs: instrument.ts imported first
- [ ] Using v8 API (`startSpan`, not `startTransaction`)
