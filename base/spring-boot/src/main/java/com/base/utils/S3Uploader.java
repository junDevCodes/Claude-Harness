package com.base.utils;

import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;
import software.amazon.awssdk.core.sync.RequestBody;
import software.amazon.awssdk.services.s3.S3Client;
import software.amazon.awssdk.services.s3.model.DeleteObjectRequest;
import software.amazon.awssdk.services.s3.model.PutObjectRequest;
import software.amazon.awssdk.services.s3.presigner.S3Presigner;
import software.amazon.awssdk.services.s3.presigner.model.PutObjectPresignRequest;

import java.net.URL;
import java.time.Duration;
import java.util.Map;
import java.util.UUID;

@Slf4j
@Component
@RequiredArgsConstructor
public class S3Uploader {

    private final S3Client s3Client;
    private final S3Presigner s3Presigner;

    @Value("${aws.s3.bucket}")
    private String bucket;

    @Value("${aws.region}")
    private String region;

    /**
     * 서버 측에서 직접 파일 업로드 (바이트 배열)
     */
    public String upload(byte[] data, String directory, String originalFilename) {
        String extension = extractExtension(originalFilename);
        String key = directory + "/" + UUID.randomUUID() + "." + extension;

        s3Client.putObject(
                PutObjectRequest.builder()
                        .bucket(bucket)
                        .key(key)
                        .build(),
                RequestBody.fromBytes(data)
        );

        log.info("S3 upload success: {}", key);
        return buildUrl(key);
    }

    /**
     * Presigned PUT URL 생성 (클라이언트 직접 업로드용)
     * 클라이언트는 반환된 URL로 HTTP PUT 요청 전송
     */
    public PresignedUrlResponse generatePresignedPutUrl(String directory,
                                                         String originalFilename,
                                                         String contentType,
                                                         int expiryMinutes) {
        String extension = extractExtension(originalFilename);
        String key = directory + "/" + UUID.randomUUID() + "." + extension;

        PutObjectPresignRequest presignRequest = PutObjectPresignRequest.builder()
                .signatureDuration(Duration.ofMinutes(expiryMinutes))
                .putObjectRequest(req -> req
                        .bucket(bucket)
                        .key(key)
                        .contentType(contentType)
                )
                .build();

        URL presignedUrl = s3Presigner.presignPutObject(presignRequest).url();

        return new PresignedUrlResponse(presignedUrl.toString(), buildUrl(key), key);
    }

    /**
     * S3 객체 삭제
     */
    public void delete(String fileUrl) {
        String key = extractKeyFromUrl(fileUrl);
        s3Client.deleteObject(DeleteObjectRequest.builder()
                .bucket(bucket)
                .key(key)
                .build());
        log.info("S3 delete success: {}", key);
    }

    private String buildUrl(String key) {
        return String.format("https://%s.s3.%s.amazonaws.com/%s", bucket, region, key);
    }

    private String extractExtension(String filename) {
        int dotIndex = filename.lastIndexOf('.');
        return dotIndex >= 0 ? filename.substring(dotIndex + 1).toLowerCase() : "bin";
    }

    private String extractKeyFromUrl(String url) {
        String prefix = String.format("https://%s.s3.%s.amazonaws.com/", bucket, region);
        return url.startsWith(prefix) ? url.substring(prefix.length()) : url;
    }

    public record PresignedUrlResponse(
            String uploadUrl,
            String fileUrl,
            String key
    ) {}
}
