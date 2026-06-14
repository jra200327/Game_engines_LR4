#include "CameraService.h"
#include <cmath>

namespace Editor {

    CameraService::CameraService(sf::RenderWindow& window, const sf::Vector2f& size)
        : _window(window)
    {
        _view = sf::View(sf::FloatRect({0.f, 0.f}, {size.x, size.y}));
    }

    sf::Vector2f CameraService::WindowToWorldPosition(const sf::Vector2i& mousePos) const
    {
        return _window.mapPixelToCoords(mousePos, _view);
    }

    void CameraService::StartMoving(const sf::Vector2i& mousePos)
    {
        _isDragging = true;
        _lastMousePos = mousePos;
    }

    void CameraService::StopMoving()
    {
        _isDragging = false;
    }

    void CameraService::Drag(const sf::Vector2i& mousePos, float deltaTime)
    {
        if (!_isDragging)
            return;

        // Вычисляем смещение в пикселях
        sf::Vector2i delta = _lastMousePos - mousePos;
        
        // Преобразуем смещение в мировые координаты с учётом зума
        float zoom = _view.getSize().x / _window.getSize().x;
        sf::Vector2f worldDelta(
            static_cast<float>(delta.x) * zoom,
            static_cast<float>(delta.y) * zoom
        );

        // Двигаем камеру
        _view.move(worldDelta);
        
        _lastMousePos = mousePos;
    }

    void CameraService::Zoom(float scrollDelta, const sf::Vector2i& mousePos)
    {
        // Получаем мировые координаты точки под мышью ДО зума
        sf::Vector2f beforeZoom = WindowToWorldPosition(mousePos);

        // Определяем коэффициент зума
        float zoomFactor = (scrollDelta > 0) ? (1.f / ZOOM_STEP) : ZOOM_STEP;
        
        // Текущий зум
        float currentZoom = _view.getSize().x / _window.getSize().x;
        float newZoom = currentZoom * zoomFactor;

        // Ограничиваем зум
        if (newZoom < ZOOM_MIN || newZoom > ZOOM_MAX)
            return;

        // Применяем зум
        _view.zoom(zoomFactor);

        // Получаем мировые координаты точки под мышью ПОСЛЕ зума
        sf::Vector2f afterZoom = WindowToWorldPosition(mousePos);

        // Корректируем позицию, чтобы точка под мышью осталась на месте
        sf::Vector2f offset = beforeZoom - afterZoom;
        _view.move(offset);
    }

    float CameraService::GetCurrentZoom() const
    {
        return _view.getSize().x / _window.getSize().x;
    }

}