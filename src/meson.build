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
  'application.cpp',

  # views
  'viewstack.cpp',
  'gameview.cpp',

  # game
  'player.cpp',

  # graphics
  'camera.cpp',
  'eventqueue.cpp',
  'font.cpp',
  'rendertarget.cpp',
  'shader.cpp',
  'sprite.cpp',
  'tilemap.cpp',
  'texture.cpp',
  'window.cpp',

  # audio
  'audiodevice.cpp',
  'soundfile.cpp',
  'soundfilewav.cpp',
  'soundfileogg.cpp',
  'soundbuffer.cpp',
  'soundsource.cpp',
  'sound.cpp',
  'soundplayer.cpp',

  # utilities / third party
  'alcheck.cpp',
  'glcheck.cpp',
  'stb_image.cpp',
  'utility.cpp',
]

exe = executable(
  'robotrampage',
  sources: srcs,
  dependencies: deps,
  install: true
)
