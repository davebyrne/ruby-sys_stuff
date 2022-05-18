# frozen_string_literal: true

require_relative "sys_stuff/version"

module SysStuff
  class Error < StandardError; end
  # Your code goes here...
  class Malloc 

    def initialize
      puts "hello from ruby"
    end

  end
end

require "sys_stuff/sys_stuff"

