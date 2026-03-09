# Quick Reference: Test Template

Complete test template showing repository and service test patterns with AAA (Arrange, Act, Assert) structure.

```python
"""Tests for a domain entity."""
import pytest
from unittest.mock import AsyncMock, MagicMock
from sqlmodel.ext.asyncio.session import AsyncSession

from backend.domain.artist.service import ArtistService
from backend.domain.artist.repository import ArtistRepository
from backend.domain.artist.model import Artist
from backend.dtos.artist import ArtistRequestDto
from backend.error import NotFoundError


@pytest.fixture
def sample_artist():
    """Fixture for sample artist data."""
    return Artist(
        id="test-artist-id",
        name="Test Artist",
        bio="Test bio"
    )


@pytest.fixture
def mock_session():
    """Fixture for mocked database session."""
    return AsyncMock(spec=AsyncSession)


class TestArtistRepository:
    """Test suite for ArtistRepository."""

    @pytest.mark.asyncio
    async def test_get_by_id_success(self, mock_session, sample_artist):
        """Test get_by_id returns artist when found."""
        # Arrange
        mock_result = MagicMock()
        mock_result.scalar_one_or_none.return_value = sample_artist
        mock_session.execute = AsyncMock(return_value=mock_result)

        repository = ArtistRepository(mock_session)

        # Act
        result = await repository.get_by_id("test-artist-id")

        # Assert
        assert result is not None
        assert result.id == sample_artist.id
        assert result.name == sample_artist.name

    @pytest.mark.asyncio
    async def test_get_by_id_not_found(self, mock_session):
        """Test get_by_id returns None when not found."""
        # Arrange
        mock_result = MagicMock()
        mock_result.scalar_one_or_none.return_value = None
        mock_session.execute = AsyncMock(return_value=mock_result)

        repository = ArtistRepository(mock_session)

        # Act
        result = await repository.get_by_id("nonexistent-id")

        # Assert
        assert result is None


class TestArtistService:
    """Test suite for ArtistService."""

    @pytest.mark.asyncio
    async def test_create_artist_success(self, mock_session, sample_artist):
        """Test create_artist creates and returns artist."""
        # Arrange
        mock_repo = AsyncMock()
        mock_repo.create = AsyncMock(return_value=sample_artist)

        service = ArtistService(mock_session)
        service._repository = mock_repo

        request_dto = ArtistRequestDto(
            name="Test Artist",
            bio="Test bio"
        )

        # Act
        result = await service.create_artist(request_dto)

        # Assert
        assert result.name == request_dto.name
        mock_repo.create.assert_awaited_once()
```
