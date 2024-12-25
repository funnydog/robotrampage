deps = []
deps += dependency('freetype2', required : true, fallback : ['freetype2', 'freetype_dep'])
deps += dependency('glew', required : true, fallback : ['glew', 'glew_dep'])
deps += dependency('glfw3', required : true, fallback : ['glfw', 'glfw_dep'])
deps += dependency('glm', required : true, fallback : ['glm', 'glm_dep'])
deps += dependency('openal', required : true, fallback : ['openal-soft', 'openal_dep'])
deps += dependency('vorbisfile')

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
