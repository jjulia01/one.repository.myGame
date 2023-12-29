internalVariable void clearScreen(u32 color) {
	u32* pixel = (u32*)renderState.memory;
	for (size_t y = 0; y < renderState.height; y++)
	{
		for (size_t x = 0; x < renderState.width; x++)
		{
			*pixel++ = color;
		}
	}
}


internalVariable void drawRectangleInPixels(int x0, int y0, int x1, int y1, u32 color) {

	x0 = clamp(0, x0, renderState.width);
	x1 = clamp(0, x1, renderState.width);
	y0 = clamp(0, y0, renderState.height);
	y1 = clamp(0, y1, renderState.height);

	for (size_t y = y0; y < y1; y++)
	{
		u32* pixel = (u32*)renderState.memory + x0 + y * renderState.width;
		for (size_t x = x0; x < x1; x++)
		{
			*pixel++ = color;
		}
	}
}

globalVariable float renderScale = 0.01f;

internalVariable void drawRectangle(float x, float y, float halfSizeX, float halfSizeY, u32 color) {
	x *= renderState.height * renderScale;
	y *= renderState.height * renderScale;
	halfSizeX *= renderState.height * renderScale;
	halfSizeY *= renderState.height * renderScale;

	x += renderState.width / 2.f;
	y += renderState.height / 2.f;

	// ������ �������� ���������
	int x0 = x - halfSizeX;
	int x1 = x + halfSizeX;
	int y0 = y - halfSizeY;
	int y1 = y + halfSizeY;
	// ������ �������������
	drawRectangleInPixels(x0, y0, x1, y1, color);
}