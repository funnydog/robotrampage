freetype2 = dependency('freetype2')
gl = dependency('gl')
glm = dependency('glm')
glew = dependency('glew')
glfw3 = dependency('glfw3')
openal = dependency('openal')
vorbisfile = dependency('vorbisfile')

deps = [
  freetype2,
  gl,
  glm,
  glew,
  glfw3,
  openal,
  vorbisfile,
]

srcs = [
  'robotrampage.cpp',
]

exe = executable(
  'robotrampage',
  sources: srcs,
  dependencies: deps,
  install: true
)
