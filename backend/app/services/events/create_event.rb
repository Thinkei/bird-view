module Events
  class CreateEvent < BaseService
    def self.call
      new.call
    end

    FIELDS = %i[
      topic
      event
      parser
      partition
      offset
      create_time
      receive_time
      uuid
      event
      data
    ]

    def initialize(params)
      @params = params
    end

    def call
      Event.create(@params.slice(*FIELDS))
    rescue ActiveRecord::RecordInvalid => err
      errors << 'Can not create event'
      log_error err
    end
  end
end
