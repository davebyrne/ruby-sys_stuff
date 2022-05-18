# frozen_string_literal: true

RSpec.describe SysStuff do
  it "has a version number" do
    expect(SysStuff::VERSION).not_to be nil
  end

  it "does something useful" do
    malloc = SysStuff::Posix::Malloc.new
    malloc.alloc(10000)
    malloc.free

    sem = SysStuff::Posix::NamedSemaphore.new
    sem.hello
  end
end
