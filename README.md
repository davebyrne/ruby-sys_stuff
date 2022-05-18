# SysStuff

Gem to hold support for POSIX/SysV functions that are not included in the Ruby stdlib.  

Currently only POSIX Semaphores `sem_post/sem_wait` are implemented. 

This gem is not published to ruby gems and must be installed via git.


## Installation

Add this line to your application's Gemfile:

```ruby
gem 'sys_stuff', git: 'https://github.com/davebyrne/ruby-sys_stuff', 
```

And then execute:

    $ bundle install


## Usage

### POSIX NamedSemaphores

POSIX Named semaphores can be used across calls to `fork` or can be referenced from unrelated processes by name.

Example Usage: 
```ruby 
semaphore = SysStuff::Posix::NamedSemaphore.create("foo")

# unlink the semaphore right away if un-related processes
# don't need to access it.  this will prevent stale semaphores
# from sticking around after the process exits

semaphore.unlink!

Process.fork do 
    # .... wait in the child process for the 
    # parent to notify via semaphore
    semaphore.wait
    # ... do stuff
end

# do stuff in the parent and then notify the child
semaphore.post

semaphore.close

# wait for children to exit
Process.waitall
```

Unrelated processes can communicate using named semaphores:
```ruby
# process-1

# create will fail if the semaphore already exists
semaphore = SysStuff::Posix::NamedSemaphore.create("foo")

# ... do stuff ... wait/post

# good practice to unlink the semaphore in the creating process
semaphore.unlink!

semaphore.close
```
```ruby
# process-2

# open an existing semaphore.  This will fail if the semaphore
# has not already been created
semaphore = SysStuff::Posix::NamedSemaphore.open("foo")

# ... do stuff ... wait/post

semaphore.close
```

An an initial value can be passed to `create` to start the semaphore with a value above 0:
```ruby
val = 10
semaphore = SysStuff::Posix::NamedSemaphore.create("foo", val)

# this will not block
(0..10).each { semaphore.wait }

# this will block
semaphore.wait
```
