#ifndef CAMERASERVICE_H
#define CAMERASERVICE_H

#include <SFML/Graphics/RenderWindow.hpp>

namespace Editor {

    class CameraService
    {
        static constexpr float DRAG_SPEED = 500.f;
        static constexpr float ZOOM_STEP = 1.1f;
        static constexpr float ZOOM_MIN = 0.3f;
        static constexpr float ZOOM_MAX = 5.f;

        sf::RenderWindow& _window;
        sf::View _view;
        
        bool _isDragging = false;
        sf::Vector2i _lastMousePos;

    public:
        CameraService(sf::RenderWindow& window, const sf::Vector2f& size);

        const sf::View& GetView() const { return _view; }
        sf::View& GetView() { return _view; }

        // Преобразование координат окна в мировые
        sf::Vector2f WindowToWorldPosition(const sf::Vector2i& mousePos) const;

        // Drag камеры
        void StartMoving(const sf::Vector2i& mousePos);
        void StopMoving();
        void Drag(const sf::Vector2i& mousePos, float deltaTime);
        bool IsDragging() const { return _isDragging; }

        // Зум
        void Zoom(float scrollDelta, const sf::Vector2i& mousePos);
        float GetCurrentZoom() const;
    };

}

#endif // CAMERASERVICE_H