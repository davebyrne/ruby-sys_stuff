# frozen_string_literal: true
require 'fileutils'

RSpec.describe SysStuff::Posix::NamedSemaphore do

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

  it "does not create a semaphore if it already exists" do 

    sem1 = SysStuff::Posix::NamedSemaphore.create("mysem")
    begin 
      expect { SysStuff::Posix::NamedSemaphore.create("mysem") }.to raise_error(/exists/)
    ensure 
      sem1.unlink!
    end

  end

  it "prevents opening a semaphore that does not exist" do 
    expect { SysStuff::Posix::NamedSemaphore.open("nothere.#{Process.pid}") }.to raise_error(/no such file or directory/i)
  end

  it "opens an existing semaphore in a new process" do 

    sem = SysStuff::Posix::NamedSemaphore.create("mysem")


    # touch a file, then remove it in the child and noitify the parent
    filename = "/tmp/rspec.#{Process.pid}"
    File.unlink(filename) if File.exist? filename

    FileUtils.touch(filename)

    Process.fork do
      # lose the reference to the parent semaphore
      sem = nil

      child_sem = SysStuff::Posix::NamedSemaphore.open("mysem")
      expect(File.exist? filename).to be_truthy
      sleep(0.3)
      File.unlink(filename)
      child_sem.post
      child_sem.close

    end
    
    expect(File.exist? filename).to be_truthy
    sem.wait
    expect(File.exist? filename).to be_falsey

    Process.waitall
    sem.unlink!
    sem.close

  end
end
