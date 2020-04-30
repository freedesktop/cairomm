# MSVC_NMake/cairomm

# Input: mm_conf_data, cairommconfig_h
# Output: cairomm_rc

cairomm_rc = configure_file(
  input: 'cairomm.rc.in',
  output: '@BASENAME@',
  configuration: mm_conf_data,
)

# Copy the generated configuration header into the MSVC project directory.
configure_file(
  input: cairommconfig_h,
  output: 'cairommconfig.h',
  copy: true,
)
