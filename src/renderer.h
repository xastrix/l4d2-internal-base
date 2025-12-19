#pragma once

#include "fonts.h"

struct vertex_t {
	float u0/* x */, u1/* y */, u2/* z */;
	float u3 /* rhw */;
	D3DCOLOR u4/* color */;
	float u5/* tu */, u6/* tv */;
};

struct sprite_t {
	void init(IDirect3DDevice9* device, const byte* img, const size_t img_size, int width, int height) {
		m_width = width;
		m_height = height;
		m_device = device;
		m_image = img;
		m_image_size = img_size;
	}

	void begin() {
		if (!m_device || !m_sprite)
			D3DXCreateSprite(m_device, &m_sprite);

		if (!m_texture)
			D3DXCreateTextureFromFileInMemoryEx(m_device, m_image, m_image_size, m_width, m_height,
				D3DX_DEFAULT, 0, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &m_texture);

		m_sprite->Begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE);
	}

	void end() {
		if (!m_device || !m_sprite)
			return;

		m_sprite->End();
	}

	void on_reset() {
		if (!m_device || !m_sprite || !m_texture)
			return;

		m_sprite->OnLostDevice();

		if (m_texture) {
			m_texture->Release();
			m_texture = nullptr;
		}
	}

	void on_reset_end() {
		if (!m_device || !m_sprite)
			return;

		m_sprite->OnResetDevice();
	}

	void draw(int x, int y, color_t color = color_t{ 255, 255, 255 }) {
		if (!m_device || !m_texture || !m_sprite)
			return;

		D3DXMATRIX matrix{};
		D3DXMatrixTranslation(&matrix, static_cast<float>(x), static_cast<float>(y), 0.0f);

		m_sprite->SetTransform(&matrix);
		m_sprite->Draw(m_texture, 0, 0, 0, color.get());
	}

	int get_width() {
		return m_width;
	}

	int get_height() {
		return m_height;
	}

private:
	int m_width{};
	int m_height{};
	IDirect3DDevice9* m_device{};
	ID3DXSprite* m_sprite{};
	IDirect3DTexture9* m_texture{};
	const byte* m_image{};
	size_t m_image_size{};
};

struct renderer {
	bool initialise(IDirect3DDevice9* device);
	
	// Create objects (device, viewport, stateblock)
	bool create_objects(IDirect3DDevice9* device);

	// Set up render states
	void begin_modificated_states();

	void draw_line(float x0, float y0, float x1, float y1, float thickness, color_t color);
	void draw_filled_rect(float x, float y, float w, float h, color_t color);
	void draw_rect(float x, float y, float w, float h, color_t color);
	void draw_corner_box(float x, float y, float w, float h, float cx, float cy, color_t color);

	// End render states
	void end_modificated_states();

	void shutdown();

private:
	IDirect3DDevice9* m_device{};
	ID3DXLine* m_line{};
	IDirect3DStateBlock9* m_block{};
};

inline renderer g_renderer{};