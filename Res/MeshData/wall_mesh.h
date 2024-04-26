const Vertex wall_vertices[] = {
  { { -1.000000f, 2.000000f, 1.000000f },  { 0.996094f, 0.492187f } },
  { { -1.000000f, 0.000000f, -1.000000f }, { 0.670573f, 0.003906f } },
  { { -1.000000f, -0.000000f, 1.000000f }, { 0.996094f, 0.003906f } },
  { { -1.000000f, 2.000000f, -1.000000f }, { 0.670573f, 0.492187f } },
  { { 1.000000f, 2.000000f, 1.000000f },   { 0.003906f, 0.503906f } },
  { { -1.000000f, 2.000000f, 1.000000f },  { 0.329427f, 0.503906f } },
  { { -1.000000f, -0.000000f, 1.000000f }, { 0.329427f, 0.992187f } },
  { { -1.000000f, 2.000000f, -1.000000f }, { 0.337240f, 0.503906f } },
  { { -1.000000f, 2.000000f, 1.000000f },  { 0.662760f, 0.503906f } },
  { { 1.000000f, 2.000000f, 1.000000f },   { 0.662760f, 0.992187f } },
  { { -1.000000f, 2.000000f, -1.000000f }, { 0.329427f, 0.492187f } },
  { { 1.000000f, 0.000000f, -1.000000f },  { 0.003906f, 0.003906f } },
  { { -1.000000f, 0.000000f, -1.000000f }, { 0.329427f, 0.003906f } },
  { { 1.000000f, 0.000000f, -1.000000f },  { 0.662760f, 0.492187f } },
  { { -1.000000f, -0.000000f, 1.000000f }, { 0.337240f, 0.003906f } },
  { { -1.000000f, 0.000000f, -1.000000f }, { 0.662760f, 0.003906f } },
  { { 1.000000f, -0.000000f, 1.000000f },  { 0.003906f, 0.992187f } },
  { { 1.000000f, -0.000000f, 1.000000f },  { 0.337240f, 0.492187f } },
  { { 1.000000f, 2.000000f, -1.000000f },  { 0.337240f, 0.992187f } },
  { { 1.000000f, 2.000000f, -1.000000f },  { 0.003906f, 0.492187f } },
  { { 1.000000f, 0.000000f, -1.000000f },  { 0.996094f, 0.503906f } },
  { { 1.000000f, 2.000000f, 1.000000f },   { 0.670573f, 0.992187f } },
  { { 1.000000f, -0.000000f, 1.000000f },  { 0.670573f, 0.503906f } },
  { { 1.000000f, 2.000000f, -1.000000f },  { 0.996094f, 0.992187f } },
};
const uint16_t wall_indices[] = {
  0, 1, 2, 0, 3, 1, 
  4, 5, 6, 7, 8, 9, 
  10, 11, 12, 13, 14, 15, 
  4, 6, 16, 13, 17, 14, 
  7, 9, 18, 10, 19, 11, 
  20, 21, 22, 20, 23, 21, 
};