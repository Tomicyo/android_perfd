
set(PERFU_SRCS
activity_manager.cc
background_queue.cc
bash_command.cc
config.cc
current_process.cc
device_info.cc
file_cache.cc
file_reader.cc
fs/dir.cc
fs/disk_file_system.cc
fs/file.cc
fs/file_system.cc
fs/memory_file_system.cc
fs/path.cc
installer.cc
package_manager.cc
process_manager.cc
socket_utils.cc
stopwatch.cc
tokenizer.cc
trace.cc
activity_manager.h
android_studio_version.h
background_queue.h
bash_command.h
circular_buffer.h
clock.h
config.h
count_down_latch.h
current_process.h
device_info.h
fake_clock.h
file_cache.h
file_reader.h
filesystem_notifier.h
fs/dir.h
fs/disk_file_system.h
fs/file.h
fs/file_system.h
fs/memory_file_system.h
fs/path.h
installer.h
log.h
package_manager.h
process_manager.h
producer_consumer_queue.h
socket_utils.h
stopwatch.h
thread_name.h
time_value_buffer.h
tokenizer.h
trace.h
trie.h
uid_fetcher.h

clock.cc
current_process.linux.cc
filesystem_notifier.cc
thread_name.cc
)

if(ANDROID)
    list(APPEND PERFU_SRCS log.android.cc log.cc)
endif()

add_library(perf_utils ${PERFU_SRCS})