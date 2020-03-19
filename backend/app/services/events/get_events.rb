module Events
  class GetEvents < BaseService
    attr_reader :data

    def call
      @data = ::Event.limit(20)
    end
  end
end
