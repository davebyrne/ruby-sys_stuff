# frozen_string_literal: true
require 'fileutils'

RSpec.describe SysStuff do
  it "has a version number" do
    expect(SysStuff::VERSION).not_to be nil
  end

  it "coordinates access between two processes" do
    sem = SysStuff::Posix::NamedSemaphore.create("mysem")
    sem.unlink!

    # touch a file, then remove it in the child and noitify the parent
    filename = "/tmp/rspec.#{Process.pid}"
    File.unlink(filename) if File.exist? filename

    FileUtils.touch(filename)
    Process.fork do
      expect(File.exist? filename).to be_truthy
      sleep(0.3)
      File.unlink(filename)
      sem.post
    end
    expect(File.exist? filename).to be_truthy
    sem.wait
    expect(File.exist? filename).to be_falsey

    Process.waitall
    sem.close
    
  end
end
