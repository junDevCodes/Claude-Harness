---
name: threejs-realtime-guidelines
description: Three.js + React Three Fiber (R3F) 기반 실시간 3D 시각화 가이드라인. Scene/Camera/Renderer 라이프사이클, useFrame 훅으로 60FPS 루프, instancedMesh로 대량 객체 렌더링, frustum culling/LOD 성능 최적화, Raycaster 인터랙션, WebSocket 실시간 데이터 바인딩(히트맵·마커·포인트클라우드). 3D 대시보드, WebGL 시각화, 실시간 데이터 렌더링 작업 시 사용.
triggers:
  - three.js
  - threejs
  - Three.js
  - WebGL
  - react-three-fiber
  - r3f
  - useFrame
  - useThree
  - drei
  - InstancedMesh
  - Raycaster
  - heatmap
  - point cloud
---

# Three.js + R3F Real-time Visualization Guidelines

## Purpose

Three.js와 React Three Fiber (R3F)를 사용한 실시간 3D 시각화의 표준 패턴.
60FPS 유지, 대량 객체 렌더링, WebSocket 데이터 바인딩, 인터랙션 처리에 초점을 맞춘다.

## When to Use This Skill

- 3D Scene 신규 구성 (Scene + Camera + Renderer)
- 실시간 데이터 시각화 (히트맵, 마커, 포인트 클라우드)
- WebSocket 스트림을 3D 객체 속성에 바인딩
- 60FPS 성능 튜닝 (frustum culling, LOD, InstancedMesh)
- 마우스/터치 인터랙션 (Raycaster, OrbitControls)
- WebGL 컨텍스트 누수 방지

## When NOT to Use This Skill

- 2D 차트 시각화 (D3/Chart.js) → 일반 React 패턴
- React Native 3D → `react-native-guidelines` + expo-three
- 정적 3D 모델 뷰어만 필요 → `<model-viewer>` 웹 컴포넌트로 충분

---

## Quick Start

### 의존성

```bash
pnpm add three @react-three/fiber @react-three/drei
pnpm add -D @types/three
```

### 최소 R3F Scene

```typescript
import { Canvas } from '@react-three/fiber';
import { OrbitControls, Stats } from '@react-three/drei';

export function Dashboard3D() {
  return (
    <Canvas
      camera={{ position: [5, 5, 5], fov: 50 }}
      gl={{ antialias: true, powerPreference: 'high-performance' }}
      dpr={[1, 2]}  // 디바이스 픽셀 비율 제한 (성능)
    >
      <ambientLight intensity={0.5} />
      <pointLight position={[10, 10, 10]} />
      <Heatmap />
      <OrbitControls enableDamping />
      {process.env.NODE_ENV === 'development' && <Stats />}
    </Canvas>
  );
}
```

---

## Pattern 1: 60FPS 루프 (`useFrame`)

### 매 프레임 업데이트

```typescript
import { useFrame } from '@react-three/fiber';
import { useRef } from 'react';
import * as THREE from 'three';

function RotatingCube() {
  const meshRef = useRef<THREE.Mesh>(null);

  useFrame((_state, delta) => {
    if (!meshRef.current) return;
    // delta = 이전 프레임과의 시간차(초). 프레임레이트 무관 일정한 회전 속도.
    meshRef.current.rotation.y += delta * 0.5;
  });

  return (
    <mesh ref={meshRef}>
      <boxGeometry args={[1, 1, 1]} />
      <meshStandardMaterial color="orange" />
    </mesh>
  );
}
```

### 핵심 결정

- `useFrame`은 60FPS(또는 모니터 주사율)로 실행 — **무거운 연산 금지**
- React state 업데이트는 피하고 ref로 직접 객체 속성 수정 (리렌더 회피)
- `delta` 사용으로 프레임레이트 비의존 애니메이션

---

## Pattern 2: InstancedMesh로 대량 객체 렌더링

### 1000개 마커를 1 draw call로

```typescript
import { useRef, useMemo, useEffect } from 'react';
import * as THREE from 'three';

const tempObject = new THREE.Object3D();

function PersonMarkers({ positions }: { positions: [number, number, number][] }) {
  const meshRef = useRef<THREE.InstancedMesh>(null);
  const count = positions.length;

  useEffect(() => {
    if (!meshRef.current) return;
    positions.forEach((pos, i) => {
      tempObject.position.set(...pos);
      tempObject.updateMatrix();
      meshRef.current!.setMatrixAt(i, tempObject.matrix);
    });
    meshRef.current.instanceMatrix.needsUpdate = true;
  }, [positions]);

  return (
    <instancedMesh ref={meshRef} args={[undefined, undefined, count]}>
      <sphereGeometry args={[0.1, 8, 8]} />
      <meshStandardMaterial color="red" />
    </instancedMesh>
  );
}
```

### 핵심 결정

- 1000개 개별 mesh = 1000 draw call → InstancedMesh = 1 draw call (1000배 빠름)
- 위치만 다르고 geometry/material 같을 때 필수
- `instanceMatrix.needsUpdate = true` 잊지 말 것

---

## Pattern 3: WebSocket 실시간 데이터 바인딩

### 히트맵 실시간 업데이트

```typescript
import { useRef, useEffect } from 'react';
import { useWebSocket } from '@/hooks/useWebSocket';
import * as THREE from 'three';

type HeatmapFrame = { values: number[][]; timestamp: number };

function LiveHeatmap({ width = 32, height = 32 }) {
  const meshRef = useRef<THREE.Mesh>(null);
  const textureRef = useRef<THREE.DataTexture | null>(null);
  const { lastMessage } = useWebSocket<HeatmapFrame>('/ws/heatmap');

  // 텍스처 1회만 생성
  useEffect(() => {
    const data = new Uint8Array(width * height * 4);
    const tex = new THREE.DataTexture(data, width, height, THREE.RGBAFormat);
    tex.needsUpdate = true;
    textureRef.current = tex;
  }, [width, height]);

  // WebSocket 데이터 → 텍스처 업데이트 (리렌더 없이)
  useEffect(() => {
    if (!lastMessage || !textureRef.current) return;
    const data = textureRef.current.image.data as Uint8Array;
    for (let y = 0; y < height; y++) {
      for (let x = 0; x < width; x++) {
        const value = lastMessage.values[y][x];
        const idx = (y * width + x) * 4;
        // value 0~1 → red 그라디언트
        data[idx + 0] = Math.floor(value * 255);
        data[idx + 1] = 0;
        data[idx + 2] = Math.floor((1 - value) * 255);
        data[idx + 3] = 255;
      }
    }
    textureRef.current.needsUpdate = true;
  }, [lastMessage, width, height]);

  return (
    <mesh ref={meshRef} rotation={[-Math.PI / 2, 0, 0]}>
      <planeGeometry args={[10, 10]} />
      <meshBasicMaterial map={textureRef.current ?? null} transparent opacity={0.7} />
    </mesh>
  );
}
```

### 핵심 결정

- React state로 텍스처 데이터 관리하지 말 것 (배열 큰 경우 GC 압박)
- DataTexture를 useRef로 보관하고 in-place 수정 → `needsUpdate = true`
- 60FPS 데이터 스트림이면 throttle/debounce 고려

---

## Pattern 4: Raycaster 인터랙션

### R3F 이벤트 핸들러 (간단)

```typescript
function ClickableSphere() {
  const [hovered, setHovered] = useState(false);
  return (
    <mesh
      onClick={(e) => {
        e.stopPropagation();
        console.log('clicked at', e.point);
      }}
      onPointerOver={() => setHovered(true)}
      onPointerOut={() => setHovered(false)}
    >
      <sphereGeometry args={[0.5]} />
      <meshStandardMaterial color={hovered ? 'hotpink' : 'orange'} />
    </mesh>
  );
}
```

### 직접 Raycaster (고급)

```typescript
import { useThree, useFrame } from '@react-three/fiber';
import * as THREE from 'three';

function CustomRaycaster() {
  const { camera, scene, pointer } = useThree();
  const raycaster = useRef(new THREE.Raycaster());

  useFrame(() => {
    raycaster.current.setFromCamera(pointer, camera);
    const intersects = raycaster.current.intersectObjects(scene.children, true);
    // intersects[0]?.object 사용
  });

  return null;
}
```

---

## Pattern 5: 성능 최적화 체크리스트

### Frustum Culling

```typescript
// Three.js는 자동으로 frustum culling 수행
mesh.frustumCulled = true; // 기본값 (그대로 두기)

// InstancedMesh는 boundingSphere 직접 설정 권장
instancedMesh.computeBoundingSphere();
```

### LOD (Level of Detail)

```typescript
import { LOD } from 'three';

const lod = new LOD();
lod.addLevel(highDetailMesh, 0);    // 0~50 거리
lod.addLevel(mediumDetailMesh, 50); // 50~150
lod.addLevel(lowDetailMesh, 150);   // 150+
scene.add(lod);
```

### 텍스처 최적화

- 2의 제곱수 (256, 512, 1024, 2048) 사용
- KTX2/Basis Universal 압축 텍스처
- `texture.minFilter = THREE.LinearMipmapLinearFilter` (mipmap)

### Geometry 공유

```typescript
// ❌ 매 컴포넌트마다 새 geometry
function Marker() {
  return <mesh geometry={new THREE.SphereGeometry(0.1)} />; // 100개면 100개 GPU 업로드
}

// ✅ 한 번만 생성
const sharedGeometry = new THREE.SphereGeometry(0.1);
function Marker() {
  return <mesh geometry={sharedGeometry} />;
}
```

---

## Pattern 6: 메모리 누수 방지

### Cleanup 의무

```typescript
useEffect(() => {
  const geometry = new THREE.BoxGeometry();
  const material = new THREE.MeshStandardMaterial();
  const mesh = new THREE.Mesh(geometry, material);
  scene.add(mesh);

  return () => {
    scene.remove(mesh);
    geometry.dispose();
    material.dispose();
    // 텍스처도 dispose() 필요
  };
}, []);
```

### R3F는 자동 cleanup 처리

```typescript
// ✅ R3F가 unmount 시 자동으로 dispose
function MyMesh() {
  return (
    <mesh>
      <boxGeometry />
      <meshStandardMaterial />
    </mesh>
  );
}
```

→ **R3F 사용을 강력 권장.** 직접 Three.js는 dispose 누락이 빈번.

---

## Anti-Patterns (피해야 할 패턴)

### ❌ useFrame 안에서 React state 업데이트

```typescript
// ❌ 60FPS로 setState → 60FPS로 리렌더 → 성능 폭망
useFrame(() => {
  setRotation(rotation + 0.01);
});
```

→ ref로 직접 mesh.rotation 수정.

### ❌ 매 프레임마다 새 객체 생성

```typescript
// ❌ GC 압박 (60FPS × 새 Vector3)
useFrame(() => {
  const target = new THREE.Vector3(0, 0, 0);
  mesh.lookAt(target);
});

// ✅ 외부에서 1회 생성
const target = new THREE.Vector3();
useFrame(() => {
  mesh.lookAt(target);
});
```

### ❌ 1000개 mesh 개별 렌더링

```typescript
// ❌ 1000 draw call
{positions.map((p, i) => (
  <mesh key={i} position={p}>
    <sphereGeometry />
  </mesh>
))}
```

→ InstancedMesh 사용 (Pattern 2).

### ❌ Canvas를 조건부 렌더링

```typescript
// ❌ tab 전환 시 WebGL 컨텍스트 매번 재생성 (느림 + 메모리 누수)
{showDashboard && <Canvas>...</Canvas>}
```

→ Canvas는 항상 마운트, 내부 컴포넌트만 토글. 또는 `frameloop="demand"`로 일시정지.

### ❌ shadowMap 무분별 활성화

```typescript
// ❌ 모든 light + 모든 mesh에 shadow
<directionalLight castShadow />
<mesh castShadow receiveShadow />
```

→ shadowMap은 매우 비쌈. 필수 객체에만, shadow.mapSize는 1024 이하.

---

## Trigger Keywords

- `three.js` / `Three.js` / `WebGL`
- `react-three-fiber` / `r3f` / `@react-three/drei`
- `useFrame` / `useThree`
- `InstancedMesh` / `Raycaster` / `OrbitControls`
- `heatmap` / `point cloud` / `3D 시각화` (도메인)

---

## Project Customization Notes

이 스킬은 **스켈레톤 버전(T3)**으로 작성되었으며, 다음 시나리오에서 보강이 필요하다:

1. **GLB/GLTF 모델 로더** — drei `useGLTF` 패턴 추가 예정
2. **포스트프로세싱** — `@react-three/postprocessing` (블룸, SSAO 등)
3. **VR/AR** — WebXR 통합 패턴 (필요 시)
4. **물리 엔진** — `@react-three/rapier` 통합 (선택)

실 프로젝트 적용 후 발견한 패턴은 `/harness-backport`로 역전파한다.

---

## Companion Skills

- **`react-vite-pwa-guidelines`** — Vite 빌드 + PWA + WebSocket 클라이언트 (병행 권장)
- **`vercel-react-best-practices`** — React 일반 성능 최적화 (병행 가능)
