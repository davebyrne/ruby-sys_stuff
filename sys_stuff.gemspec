# frozen_string_literal: true

require_relative "lib/sys_stuff/version"

Gem::Specification.new do |spec|
  spec.name          = "sys_stuff"
  spec.version       = SysStuff::VERSION
  spec.authors       = ["Dave Byrne"]
  spec.email         = ["davebyrne82@gmail.com"]

  spec.summary       = "Support for POSIX and SysV functions missing from the Ruby stdlib."
  spec.description   = "Missing POSIX and SysV support for semaphores and more."
  spec.homepage      = "https://github.com/davebyrne/ruby-sys_stuff"
  spec.required_ruby_version = ">= 2.4.0"


  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = spec.homepage

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(File.expand_path(__dir__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{\A(?:test|spec|features)/}) }
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

  spec.add_development_dependency 'rake-compiler', '~> 1.2'
  spec.extensions = %w[ext/my_foo/extconf.rb]

  # Uncomment to register a new dependency of your gem
  # spec.add_dependency "example-gem", "~> 1.0"

  # For more information and examples about making a new gem, checkout our
  # guide at: https://bundler.io/guides/creating_gem.html
end
