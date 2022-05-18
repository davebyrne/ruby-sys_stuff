module SysStuff
  module Posix

    class NamedSemaphore

      class << self

        def create(name, value=0)

          sem = NamedSemaphore.new(name)
          sem.create(value)

          sem
        end

        def open(name)

          sem = NamedSemaphore.new(name)
          sem.open()

          sem
        end

      end      

      def initialize(name)
        @name = name
      end

    end
  end
end