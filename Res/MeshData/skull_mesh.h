const Vertex skull_vertices[] = {
  { { 0.050541f, -0.084150f, -0.000119f }, { 0.112666f, 0.785586f } },
  { { 0.104845f, -0.085079f, -0.000199f }, { 0.004934f, 0.784697f } },
  { { 0.056182f, -0.084248f, 0.069434f }, { 0.030321f, 0.648521f } },
  { { -0.010753f, -0.013074f, -0.000024f }, { 0.267813f, 0.787131f } },
  { { -0.004590f, -0.083206f, -0.069597f }, { 0.136183f, 0.938482f } },
  { { -0.004383f, -0.083212f, 0.069525f }, { 0.138696f, 0.634094f } },
  { { 0.055976f, -0.084242f, -0.069687f }, { 0.028061f, 0.922268f } },
  { { 0.055976f, -0.084242f, -0.069687f }, { 0.685905f, 0.025341f } },
  { { 0.112267f, -0.001860f, -0.000206f }, { 0.500374f, 0.212172f } },
  { { 0.104845f, -0.085079f, -0.000199f }, { 0.500374f, 0.036197f } },
  { { 0.056182f, -0.084248f, 0.069434f }, { 0.314844f, 0.025341f } },
  { { 0.058624f, -0.000945f, 0.084631f }, { 0.296172f, 0.208007f } },
  { { -0.004383f, -0.083212f, 0.069525f }, { 0.180884f, 0.005320f } },
  { { -0.000650f, 0.195580f, -0.000030f }, { 0.828515f, 0.795294f } },
  { { 0.045770f, 0.165966f, 0.080958f }, { 0.991186f, 0.894768f } },
  { { 0.092441f, 0.165170f, -0.000170f }, { 0.828515f, 0.982642f } },
  { { -0.059144f, 0.167760f, 0.081115f }, { 0.991186f, 0.695820f } },
  { { -0.106058f, 0.168563f, 0.000126f }, { 0.828515f, 0.607946f } },
  { { -0.059386f, 0.167767f, -0.081002f }, { 0.665843f, 0.695820f } },
  { { 0.045529f, 0.165973f, -0.081159f }, { 0.665843f, 0.894768f } },
  { { 0.122414f, 0.081312f, -0.000217f }, { 0.500374f, 0.389522f } },
  { { 0.045770f, 0.165966f, 0.080958f }, { 0.299533f, 0.573335f } },
  { { 0.060084f, 0.082375f, 0.108128f }, { 0.278524f, 0.388153f } },
  { { -0.059144f, 0.167760f, 0.081115f }, { 0.067755f, 0.603415f } },
  { { 0.092441f, 0.165170f, -0.000170f }, { 0.500374f, 0.568128f } },
  { { 0.059762f, 0.082385f, -0.108379f }, { 0.722225f, 0.388153f } },
  { { 0.045529f, 0.165973f, -0.081159f }, { 0.701216f, 0.573335f } },
  { { 0.058373f, -0.000937f, -0.084884f }, { 0.704577f, 0.208007f } },
  { { -0.076292f, 0.080940f, 0.108331f }, { 0.012044f, 0.389001f } },
  { { -0.029952f, 0.000570f, 0.084763f }, { 0.102785f, 0.189957f } },
  { { -0.029952f, 0.000570f, 0.084763f }, { 0.300790f, 0.636063f } },
  { { -0.076292f, 0.080940f, 0.108331f }, { 0.473925f, 0.592866f } },
  { { -0.059144f, 0.167760f, 0.081115f }, { 0.645322f, 0.615837f } },
  { { -0.106058f, 0.168563f, 0.000126f }, { 0.651820f, 0.790534f } },
  { { -0.139291f, 0.061662f, 0.000171f }, { 0.476483f, 0.789087f } },
  { { -0.095921f, 0.001699f, 0.000103f }, { 0.376730f, 0.788263f } },
  { { -0.076613f, 0.080950f, -0.108175f }, { 0.470686f, 0.985238f } },
  { { -0.059386f, 0.167767f, -0.081002f }, { 0.642439f, 0.965100f } },
  { { -0.030204f, 0.000577f, -0.084753f }, { 0.298288f, 0.939189f } },
  { { -0.059386f, 0.167767f, -0.081002f }, { 0.932994f, 0.603415f } },
  { { -0.076613f, 0.080950f, -0.108175f }, { 0.988705f, 0.389001f } },
  { { -0.030204f, 0.000577f, -0.084753f }, { 0.897963f, 0.189957f } },
  { { -0.004590f, -0.083206f, -0.069597f }, { 0.819865f, 0.005320f } },
};
const uint16_t skull_indices[] = {
  0, 1, 2, 3, 4, 0, 
  0, 2, 5, 0, 4, 6, 
  5, 3, 0, 0, 6, 1, 
  7, 8, 9, 8, 10, 9, 
  11, 12, 10, 8, 11, 10, 
  13, 14, 15, 13, 16, 14, 
  13, 17, 16, 13, 18, 17, 
  13, 19, 18, 13, 15, 19, 
  20, 21, 22, 22, 21, 23, 
  20, 24, 21, 25, 24, 20, 
  25, 26, 24, 20, 11, 8, 
  27, 20, 8, 27, 25, 20, 
  20, 22, 11, 22, 23, 28, 
  22, 29, 11, 22, 28, 29, 
  5, 30, 3, 11, 29, 12, 
  31, 32, 33, 31, 33, 34, 
  31, 35, 30, 31, 34, 35, 
  33, 36, 34, 33, 37, 36, 
  35, 34, 36, 35, 3, 30, 
  38, 3, 35, 35, 36, 38, 
  3, 38, 4, 39, 25, 40, 
  39, 26, 25, 41, 40, 25, 
  42, 27, 7, 42, 41, 27, 
  7, 27, 8, 41, 25, 27, 
};